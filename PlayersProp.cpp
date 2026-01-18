#include <vector>
#include "PlayersProp.h"
#include "Room.h"
#include "player.h"

using std::vector;

vector<Player*> PlayersProp::get_players() const {
	vector<Player*> players;
	for (MapObject* obj : room.map_objects) {
		Player* p = dynamic_cast<Player*>(obj);
		if (p != nullptr) players.push_back(p);
	}
	return players;
}

vector<MapObject*> PlayersProp::get_objects() const {
	vector<Player*> players = get_players();
	vector<MapObject*> objs;

	for (auto& p : players) {
		if (p->collectible != nullptr)
			objs.push_back((MapObject*)(p->collectible));
	}

	return objs;
}

bool PlayersProp::remove_object(MapObject* obj) {
	Player* p = dynamic_cast<Player*>(obj);
	if (p) { 
		p->lives--;
		room.runner->log_event(new LostLife(obj, 1));
		if (p->lives > 0) {
			p->setPosition((room.p_doors.entry_point) ? room.p_doors.entry_point->getPosition() : V(0, 0));
			room.add_object(p);
		}
		return true;
	}
	return false;
}
