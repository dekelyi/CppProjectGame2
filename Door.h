#pragma once
#include "Object.h"

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
	DoorState state = DoorState::UNLOCKED;

	Door(V _pos, V _size, DoorDest _type) : MapObject(_pos, _size, 'D'), dest(_type) { attr = A_FOREGROUND_GREEN; }

	inline void setLocked(bool locked) {
		state = locked ? DoorState::LOCKED : DoorState::UNLOCKED;
		attr = locked ? A_FOREGROUND_RED : A_FOREGROUND_GREEN;
		this->draw();
	}

	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, V dir) override;
};