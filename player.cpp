#include "Player.h"
#include "Collectible.h"
#include "Room.h"


void Player::dump_collectible(GameRoom* room) {
	if (collectible == nullptr) return;
	collectible->setPosition(this->getPosition() + this->getDumpPosition(room));
	room->addObject(collectible);
	collectible->draw();
	collectible = nullptr;
}

V Player::getDumpPosition(GameRoom* room) {
	std::vector<V> arr = { V(0,-1), V(0,1), V(-1,0), V(1,0) };
	arr.erase(std::remove(arr.begin(), arr.end(), this->direction), arr.end());
	for (V v : arr)
		if (room->get_object_at(this->getPosition() + v) == nullptr)
			return v;
	return V(0, 0);
}