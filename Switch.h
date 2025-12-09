#pragma once
#include "prelude.h"
#include "Object.h"
#include "Door.h"
#include <functional>

typedef std::function<void(bool)> CallbackFn;

CallbackFn SwitchDoor(Door* door, Condition* condition_switch) {
	return [=](bool switched) {
		bool last_state = door->isLocked();
		condition_switch->collected = (int)switched;
		if (door->isLocked() != last_state) {
			door->draw();
		}
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
	Switch(V _pos, CallbackFn cb = nullptr) : MapObject(_pos, S1, (char)ObjType::SWITCH_OFF), callback(cb) { attr = A_FOREGROUND_YELLOW; }

	inline void setSwitch(bool val) {
		if (val == switched) return;
		this->clear();
		switched = val;
		if (callback) callback(switched);
		setGlyph(val ? (char)ObjType::SWITCH_ON : (char)ObjType::SWITCH_OFF);
		this->draw();
	}

	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override {
		switchers.push_back({ other, other->getPosition() });
		setSwitch(true);
		return CANT_MOVE;
	}

	virtual void handle_tick(GameRoom* room) override {
		if (switched) {
			switchers.remove_if([](const Switcher& s) { return s.pos != s.obj->getPosition(); });
			if (switchers.empty()) setSwitch(false);
		}
		MapObject::handle_tick(room);
	}
};

#pragma once
