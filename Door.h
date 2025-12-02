#pragma once
#include "Object.h"
#include "Room.h"

enum class DoorState {
	UNLOCKED,
	LOCKED
};

enum class DoorDest {
	NEXT,
	PREV
};

class Door : public MapObject {
	DoorDest dest;
public:
	DoorState state = DoorState::UNLOCKED;

	Door(V _pos, V _size, DoorDest _type) : MapObject(_pos, _size, 'D'), dest(_type) {}
	void add_to_room(GameRoom* room);

	virtual bool handle_collision(GameRoom* room, MapObject* other, V dir) override;
};