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
	default: return "";
	}
}

class Room;

/**
 * Door object that can transport players between rooms and be locked by
 * conditions (keys or switches).
 */
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

	/** Return true when any condition still lacks required collected items. */
	inline bool isLocked() const {
		for (auto c : conditions)
			if (c->collected < c->required)
				return true;
		return false;
	}

	/** Get a user-friendly message describing why door is locked. */
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