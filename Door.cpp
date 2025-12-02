#pragma once
#include "Door.h"
#include "Room.h"

void Door::add_to_room(GameRoom* room) {
	room->addObject(this);
	if (dest == DoorDest::NEXT)
		room->exit_point = this;
	else
		room->entry_point = this;
}

bool Door::handle_collision(GameRoom* room, MapObject* other, V dir) {
	// If door is locked, block passage
	if (state == DoorState::LOCKED)
		return false;

	GameRoom* other_room = (dest == DoorDest::NEXT) ? room->next : room->prev;
	Door* other_door = (dest == DoorDest::NEXT) ? other_room->entry_point : other_room->exit_point;

	room->removeObject(other);
	other->clear();
	other_room->addObject(other);
	other->setPosition(other_door->getPosition() + dir);
	room->last_moved_through = this;
	return true;
}