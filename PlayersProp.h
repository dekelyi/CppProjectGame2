#pragma once
#include <set>
#include "BaseProp.h"
#include "player.h"

using std::set;

class PlayersProp : public BaseProp {
public:
	set<Player*> get_players() const;

	inline int count_players() const {
		return (int)get_players().size();
	};

	set<MapObject*> get_objects() const;
	bool remove_object(MapObject* obj);
};