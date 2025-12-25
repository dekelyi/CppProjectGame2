#include "Collectible.h"
#include "Player.h"
#include "Room.h"

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
	if (!room->is_dark) return;

	struct TMP {
		MapObject* obj;
		VS dims;
	};
	std::vector<TMP> to_redraw;
	for (MapObject* obj : room->map_objects) {
		VS dim_area = is_in_bounds_dim(obj->getPosition(), this->getPosition(), obj->getSize(), area);
		to_redraw.push_back({ obj, dim_area });
	}
	for (TMP obj : to_redraw) {
		room->clear(*obj.obj, A_BACKGROUND_GREY);
		room->draw(*obj.obj, &obj.dims);
	}
}