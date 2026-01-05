#pragma once
#include <vector>
#include "BaseProp.h"
#include "player.h"

using std::vector;

class PlayersProp : public BaseProp {
public:
	vector<Player*> get_players() const;

	inline int count_players() const {
		return (int)get_players().size();
	};

	vector<MapObject*> get_objects() const;
	bool remove_object(MapObject* obj);
};