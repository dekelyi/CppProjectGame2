#include <vector>
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

std::vector<MapObject*> PlayersProp::get_objects() const {
	std::vector<Player*> players = get_players();
	std::vector<MapObject*> objs(players.size());

	for (auto& p : players) {
		if (p->collectible != nullptr)
			objs.push_back((MapObject*)(p->collectible));
	}

	return objs;
}

bool PlayersProp::handle_remove_obj(MapObject* obj) {
	Player* p = dynamic_cast<Player*>(obj);
	if (!p) return true;
	return false;
}
