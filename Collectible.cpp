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

void Torch::handle_collector_movement(GameRoom* room, V old_pos, V new_pos) {
	struct TMP {
		MapObject* obj;
		VS dims;
	};
	std::vector<TMP> to_redraw;
	for (MapObject* obj : room->map_objects) {
		VS was_in_area = is_in_bounds_dim(obj->getPosition(),old_pos, obj->getSize(), area);
		VS is_in_area = is_in_bounds_dim(obj->getPosition(),new_pos, obj->getSize(), area);
		if (was_in_area.size != is_in_area.size) {
			to_redraw.push_back({ obj, is_in_area });
		}
	}
	for (TMP obj : to_redraw) {
		room->clear(*obj.obj,A_BACKGROUND_GREY);
		room->draw(*obj.obj, &obj.dims);
	}
}
