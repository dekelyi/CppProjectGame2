#include "Collectible.h"
#include <vector>
#include "Room.h"

bool Collectible::handle_collision(GameRoom* room, MapObject* other, V dir) {
	// If collided with a player, give the collectible to the player
	Collector* p = dynamic_cast<Collector*>(other);
	if (p != nullptr && p->collectible == nullptr) {
		p->collectible = this;
		room->removeObject(this);
		return true;
	}
	return false;
}


void Collector::dump_collectible(GameRoom* room) {
	if (collectible == nullptr) return;
	collectible->setPosition(this->getPosition() + this->getFreeNextPositon(room, {}));
	room->addObject(collectible);
	collectible->draw();
	collectible = nullptr;
}

V Collector::getFreeNextPositon(GameRoom* room, std::vector<V> vnot) {
	std::vector<V> arr = { V(0,-1), V(0,1), V(-1,0), V(1,0) };
	for (V vn : vnot)
		arr.erase(std::remove(arr.begin(), arr.end(), vn), arr.end());
	for (V v : arr)
		if (room->get_object_at(this->getPosition() + v) == nullptr)
			return v;
}