#include "Collectible.h"
#include "Player.h"
#include "Room.h"

bool Collectible::handle_collision(GameRoom* room, MapObject* other, V dir) {
	// If collided with a player, give the collectible to the player
	Player* p = dynamic_cast<Player*>(other);
	if (p != nullptr && p->collectible == nullptr) {
		p->collectible = this;
		room->removeObject(this);
		return true;
	}
	return false;
}