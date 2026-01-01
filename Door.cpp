#pragma once
#include "Door.h"
#include "Collectible.h"
#include "Room.h"

M_CODE Door::handle_collision(GameRoom* room, MapObject* obj, Move& move) {
	// If door is locked, block passage
	if (isLocked()) {
		if (try_unlock(obj)) {
			room->draw(*this);
		}
		else {
			room->msg = new Msg(getMsg(), MSG_TICKS);
			return CANT_MOVE;
		}
	}

	// get in only in the direction of the door
	if (!this->size.is_ortho_direction(move.dir)) {
		return CANT_MOVE;
	}

	// redraw
	room->remove_object(obj);
	room->clear(*obj);

	// move object to next/prev room
	GameRoom* other_room = (dest == DoorDest::NEXT) ? room->next : room->prev;
	if (other_room == nullptr) return MOVED;
	Door* other_door = (dest == DoorDest::NEXT) ? other_room->p_doors.entry_point : other_room->p_doors.exit_point;

	other_room->add_object(obj);
	obj->setPosition(other_door ? other_door->getPosition() : V(0,0));
	room->p_doors.last_moved_through = this;

	// move the object in the next room
	obj->try_move(other_room, move);
	return MOVED;
}

bool Door::try_unlock(MapObject* obj) {
	// collect keys
	Player* p = dynamic_cast<Player*>(obj);
	if (p && p->collectible) {
		Key* key = dynamic_cast<Key*>(p->collectible);
		if (key) {
			delete p->collectible;
			p->collectible = nullptr;
			conditions.front()->collected++;
			return !isLocked();
		}
	}
	return false;
}