#include <set>
#include "PlayersProp.h"
#include "Room.h"
#include "player.h"

using std::set;

set<Player*> PlayersProp::get_players() const {
	set<Player*> players;
	for (MapObject* obj : room.map_objects) {
		Player* p = dynamic_cast<Player*>(obj);
		if (p != nullptr) players.insert(p);
	}
	return players;
}

set<MapObject*> PlayersProp::get_objects() const {
	set<Player*> players = get_players();
	set<MapObject*> objs;

	for (auto& p : players) {
		if (p->collectible != nullptr)
			objs.insert((MapObject*)(p->collectible));
	}

	return objs;
}

bool PlayersProp::remove_object(MapObject* obj) {
	Player* p = dynamic_cast<Player*>(obj);
	if (p) { 
		p->lives--;
		if (p->lives > 0) {
			p->setPosition((room.p_doors.entry_point) ? room.p_doors.entry_point->getPosition() : V(0, 0));
			room.add_object(p);
		}
		return true;
	}
	return false;
}
