#pragma once
#include <vector>
#include "Object.h"
#include "player.h"

class GameRoom {
	friend class GameView;

	std::vector<MapObject*> map_objects;

public:
	Player* player1;
	Player* player2;

	GameRoom(Player* _player1, Player* _player2) : player1(_player1), player2(_player2) {
		addObject(player1);
		addObject(player2);
	}

	inline void addObject(MapObject* obj) {
		map_objects.push_back(obj);
	}

	inline void removeObject(MapObject* obj) {
		map_objects.erase(std::remove(map_objects.begin(), map_objects.end(), obj));
	}

	MapObject* get_object_at(V pos) {
		for (MapObject* obj : map_objects)
			if (obj->is_at(pos))
				return obj;
		return nullptr;
	}
};