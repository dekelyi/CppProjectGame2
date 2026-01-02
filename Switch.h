#pragma once
#include "prelude.h"
#include "Object.h"
#include "Door.h"
#include "Room.h"
#include <functional>

typedef std::function<void(bool)> CallbackFn;

inline CallbackFn SwitchDoor(GameRoom* room, Door* door, Condition* condition_switch) {
	return [=](bool switched) {
		bool last_state = door->isLocked();
		condition_switch->collected = (int)switched;
	};
}

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

	inline void setSwitch(GameRoom* room, bool val) {
		if (val == switched) return;
		room->clear(*this);
		switched = val;
		if (callback) callback(switched);
		setGlyph(val ? (char)ObjType::SWITCH_ON : (char)ObjType::SWITCH_OFF);
	}

	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override {
		switchers.push_back({ other, other->getPosition() });
		setSwitch(room, true);
		return CANT_MOVE;
	}

	inline virtual void handle_tick(GameRoom* room) override {
		MapObject::handle_tick(room);
		if (switched) {
			switchers.remove_if([](const Switcher& s) { return s.pos != s.obj->getPosition(); });
			if (switchers.empty()) setSwitch(room, false);
		}
	}
};