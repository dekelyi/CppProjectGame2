#pragma once
#include "Object.h"
#include <format>

enum class DoorState {
	UNLOCKED,
	LOCKED
};

enum class DoorDest {
	NEXT,
	PREV
};

struct Condition {
	enum Kind { KEYS, SWITCH } kind;
	int required;
	int collected = 0;
};

inline std::string get_condition_str(Condition& c) {
	switch (c.kind) {
	case Condition::KEYS: return "keys";
	case Condition::SWITCH: return "switches";
	}
}

class Room;

class Door : public MapObject {
public:
	S size;
	const DoorDest dest;
	std::list<Condition*> conditions = { new Condition(Condition::KEYS, 0) };

	inline Door(V _pos, S _size, DoorDest _type, unsigned short keys=0)
		: MapObject(_pos, _size, 'D'), dest(_type), size(_size) {
		conditions.front()->required = keys;
	}

	inline ~Door() {
		for (auto c : conditions) delete c;
	}

	inline bool isLocked() const {
		for (auto c : conditions)
			if (c->collected < c->required)
				return true;
		return false;
	}

	inline std::string getMsg() const {
		for (auto c : conditions)
			if (c->collected < c->required)
				return std::format("You need {} more {} to open the door", c->required - c->collected, get_condition_str(*c));
		return "";
	}

	virtual inline std::string getAttr() const override {
		return (isLocked()) ? A_FOREGROUND_RED : A_FOREGROUND_GREEN;
	}

	bool try_unlock(MapObject* obj);
	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override;
};