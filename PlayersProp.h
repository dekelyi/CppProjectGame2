#pragma once
#include "BaseProp.h"
#include "player.h"
#include <vector>

class PlayersProp : public BaseProp {
public:
	std::vector<Player*> get_players() const;

	inline int count_players() const {
		return (int)get_players().size();
	};

	std::vector<MapObject*> get_objects() const;
	bool handle_remove_obj(MapObject* obj);
};