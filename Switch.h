#pragma once
#include "ObjTypes.h"
#include "Object.h"
#include "Door.h"
#include "Room.h"
#include <functional>
#include "EventLogger.h"

typedef std::function<void(bool)> CallbackFn;

// Event emitted when a switch changes state
class SwitchToggled : public Event {
public:
    bool on;
    SwitchToggled(const MapObject* _actor, bool _on) : Event(_actor), on(_on) {}
    virtual string to_string() override {
        return std::format("OBJ {} SWITCHED: {}", actor->getGlyph(), on ? "ON" : "OFF");
    }
};

/**
 * Create a callback that toggles a door's condition when a switch is toggled.
 * The returned lambda captures pointers to the room, door and condition to modify.
 */
inline CallbackFn SwitchDoor(GameRoom* room, Door* door, Condition* condition_switch) {
	return [=](bool switched) {
		bool last_state = door->isLocked();
		condition_switch->collected = (int)switched;
	};
}

/**
 * Switch object that can be toggled when an object stays on it. Optionally
 * executes a callback when toggled (e.g. opening a door).
 */
class Switch : public MapObject {
	bool switched = false;
	CallbackFn callback;

	struct Switcher {
		MapObject* obj;
		V pos;
	};

	std::list<Switcher> switchers;
public:
	Switch(V _pos) : MapObject(_pos, S1, (char)ObjType::SWITCH_OFF) { attr = A_FOREGROUND_YELLOW; }
	Switch(V _pos, CallbackFn cb) : Switch(_pos) {
		callback = cb;
	}

	/**
	 * Set the switch state and invoke callback if present.
	 */
	inline void setSwitch(GameRoom* room, bool val) {
		if (val == switched) return;
		room->clear(*this);
		switched = val;
		if (callback) callback(switched);
		setGlyph(val ? (char)ObjType::SWITCH_ON : (char)ObjType::SWITCH_OFF);
		// emit event
		room->runner->handle_event(new SwitchToggled(this, switched));
	}

	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override {
		switchers.push_back({ other, other->getPosition() });
		setSwitch(room, true);
		return M_CODE::CANT_MOVE;
	}

	inline virtual void handle_tick(GameRoom* room) override {
		MapObject::handle_tick(room);
		if (switched) {
			switchers.remove_if([](const Switcher& s) { return s.pos != s.obj->getPosition(); });
			if (switchers.empty()) setSwitch(room, false);
		}
	}
};