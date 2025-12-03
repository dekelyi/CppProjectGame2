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

class Room;

class Door : public MapObject {
public:
	const DoorDest dest;
	const unsigned short keys_required = 0;
	unsigned short keys_collected = 0;

	Door(V _pos, V _size, DoorDest _type, unsigned short keys=0)
		: MapObject(_pos, _size, 'D'), dest(_type), keys_required(keys) {
	}

	inline bool isLocked() const {
		return keys_required > 0 && keys_collected != keys_required;
	}

	inline std::string getMsg() const {
		if (keys_required > 0 && keys_collected != keys_required)
			return std::format("You need {} more keys to open the door", keys_required - keys_collected);
		return "";
	}

	virtual inline std::string getAttr() const override {
		return (isLocked()) ? A_FOREGROUND_RED : A_FOREGROUND_GREEN;
	}

	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, V dir) override;
};