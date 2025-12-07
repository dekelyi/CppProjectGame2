#pragma once
#include "Door.h"
#include "Collectible.h"
#include "Room.h"

M_CODE Door::handle_collision(GameRoom* room, MapObject* obj, Move& move) {
	// If door is locked, block passage
	if (isLocked()) {
		if (try_unlock(obj)) {
			this->draw();
		}
		else {
			room->msg = getMsg();
			return CANT_MOVE;
		}
	}

	if (!this->size.is_ortho_direction(move.dir)) {
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

	obj->try_move(other_room, move);
	return MOVED;
}

bool Door::try_unlock(MapObject* obj) {
	Player* p = dynamic_cast<Player*>(obj);
	if (p && p->collectible) {
		Key* key = dynamic_cast<Key*>(p->collectible);
		if (key) {
			delete p->collectible;
			p->collectible = nullptr;
			keys_collected++;
			if (keys_collected == keys_required) return true;
		}
	}
	return false;
}