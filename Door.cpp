#pragma once
#include "Door.h"
#include "Room.h"

bool Door::handle_collision(GameRoom* room, MapObject* other, V dir) {
	// If door is locked, block passage
	if (state == DoorState::LOCKED)
		return false;

	room->removeObject(other);
	other->clear();

	GameRoom* other_room = (dest == DoorDest::NEXT) ? room->next : room->prev;
	if (other_room == nullptr) return true;
	Door* other_door = (dest == DoorDest::NEXT) ? other_room->entry_point : other_room->exit_point;

	other_room->addObject(other);
	other->setPosition(other_door->getPosition() + dir);
	room->last_moved_through = this;
	return true;
}