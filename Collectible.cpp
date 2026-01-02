#include <set>
#include "Collectible.h"
#include "Player.h"
#include "Room.h"

using std::set;

M_CODE Collectible::handle_collision(GameRoom* room, MapObject* other, Move& move) {
	// If collided with a player, give the collectible to the player
	Player* p = dynamic_cast<Player*>(other);
	if (p != nullptr && p->collectible == nullptr) {
		p->collectible = this;
		room->remove_object(this, false);
		return CAN_MOVE;
	}
	return CANT_MOVE;
}

void Bomb::handle_dump(GameRoom* room) {
	this->bomb_timer = BOMB_TIMER;
}

void Bomb::handle_tick(GameRoom* room) {
	MapObject::handle_tick(room);
	if (bomb_timer == 0) {
		do_bomb(room);
		room->clear(*this);
		room->remove_object(this);
	}
	else if (bomb_timer > 0) bomb_timer--;
}

void Bomb::do_bomb(GameRoom* room) const {
	set<MapObject*> to_remove;
	V start_pos = pos - V(BOMB_AREA, BOMB_AREA);
	unsigned diameter = BOMB_AREA * 2 + 1;

	for (unsigned dx = 0; dx < diameter; dx++)
		for (unsigned dy = 0; dy < diameter; dy++) {
			auto obj = room->get_object_at(start_pos + V(dx, dy));
			if (obj && obj != this) to_remove.insert(obj);
		}

	for (MapObject* obj : to_remove) {
		room->clear(*obj);
		room->remove_object(obj);
	}
}

