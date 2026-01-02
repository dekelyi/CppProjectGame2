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
		room->remove_object(this);
		return CAN_MOVE;
	}
	return CANT_MOVE;
}

void Torch::handle_tick(GameRoom* room) {
	Collectible::handle_tick(room);
	//if (!room->p_torch.is_dark) return;

	//struct TMP {
	//	MapObject* obj;
	//	VS dims;
	//};
	//std::vector<TMP> to_redraw;
	//for (MapObject* obj : room->map_objects) {
	//	VS dim_area = is_in_bounds_dim(obj->getPosition(), this->getPosition(), obj->getSize(), area);
	//	to_redraw.push_back({ obj, dim_area });
	//}
	//for (TMP obj : to_redraw) {
	//	room->clear(*obj.obj, A_BACKGROUND_GREY);
	//	room->draw(*obj.obj);
	//}
}

void Bomb::handle_dump(GameRoom* room) {
	this->bomb_timer = BOMB_TIMER;
}

void Bomb::do_bomb(GameRoom* room) const {
	set<MapObject*> to_remove;
	V start_pos = pos - V(BOMB_AREA_2, BOMB_AREA_2);

	for (unsigned dx = 0; dx < BOMB_AREA_2 * 2; dx++)
		for (unsigned dy = 0; dy < BOMB_AREA_2 * 2; dy++) {
			auto obj = room->get_object_at(start_pos + V(dx, dy));
			if (obj && obj != this) to_remove.insert(obj);
		}

	for (MapObject* obj : to_remove) {
		room->clear(*obj);
		if (room->remove_object(obj))
			delete obj;
	}
}

