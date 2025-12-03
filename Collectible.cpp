#include "Collectible.h"
#include "Player.h"
#include "Room.h"

M_CODE Collectible::handle_collision(GameRoom* room, MapObject* other, V dir, bool draw) {
	// If collided with a player, give the collectible to the player
	Player* p = dynamic_cast<Player*>(other);
	if (p != nullptr && p->collectible == nullptr) {
		p->collectible = this;
		room->removeObject(this);
		return CAN_MOVE;
	}
	return CANT_MOVE;
}