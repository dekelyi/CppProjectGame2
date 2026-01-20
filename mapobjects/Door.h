#pragma once
#include "../Object.h"
#include <format>
#include "../EventLogger.h"

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
	default: return "";
	}
}

class Room;

class Door : public MapObject {
public:
	S size;                     // door size and orientation
	const DoorDest dest;        // direction to which the door leads (next/prev room)
	std::list<Condition*> conditions = {
		new Condition{Condition::KEYS, 0}
	};

	inline Door(V _pos, S _size, DoorDest _type, unsigned short keys = 0)
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

	virtual inline const std::string getAttr() const override {
		return (isLocked()) ? A_FOREGROUND_RED : A_FOREGROUND_GREEN;
	}

	bool try_unlock(MapObject* obj);
	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override;
};

class RoomTransitionEvent : public Event {
public:
	DoorDest dir;

	RoomTransitionEvent(const MapObject* _actor, DoorDest _dir)
		: Event(_actor), dir(_dir) {}

	virtual string to_string() override {
		return std::format("OBJ {} MOVED: {}", actor->getGlyph(), (dir == DoorDest::NEXT) ? "NEXT" : "PREV");
	}
};
