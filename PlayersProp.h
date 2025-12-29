#pragma once
#include "BaseProp.h"
#include "player.h"
#include <vector>
#include <algorithm>

class PlayersProp : public BaseProp {
public:
	std::vector<Player*> get_players() const;
	inline int count_players() const {
		return (int)get_players().size();
	};

	inline std::vector<MapObject*> get_objects() const {
		std::vector<Player*> players = get_players();
		std::vector<MapObject*> objs(players.size());

		for (auto& p: players) {
			if (p->collectible != nullptr)
				objs.push_back(dynamic_cast<MapObject*>(p->collectible));
		}

		return objs;
	}
};