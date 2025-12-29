#include "PlayersProp.h"
#include "Room.h"
#include "player.h"

std::vector<Player*> PlayersProp::get_players() const {
	std::vector<Player*> players;
	for (MapObject* obj : room.map_objects) {
		Player* p = dynamic_cast<Player*>(obj);
		if (p != nullptr) players.push_back(p);
	}
	return players;
}