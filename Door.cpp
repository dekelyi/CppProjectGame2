#pragma once
#include "Door.h"
#include "Room.h"

M_CODE Door::handle_collision(GameRoom* room, MapObject* obj, V dir) {
	// If door is locked, block passage
	if (isLocked()) {
		room->msg = getMsg();
		return CANT_MOVE;
	}

	room->removeObject(obj);
	obj->clear();

	GameRoom* other_room = (dest == DoorDest::NEXT) ? room->next : room->prev;
	if (other_room == nullptr) return MOVED;
	Door* other_door = (dest == DoorDest::NEXT) ? other_room->entry_point : other_room->exit_point;

	other_room->addObject(obj);
	obj->setPosition(other_door->getPosition());
	room->last_moved_through = this;

	obj->try_move(other_room, dir);
	return MOVED;
}