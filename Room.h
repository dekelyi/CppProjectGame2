#pragma once
#include <vector>
#include "Object.h"
#include "player.h"
#include "Door.h"

class Door;

class GameRoom {
	friend class GameView;

	std::vector<MapObject*> map_objects;

public:
	GameRoom* next;
	GameRoom* prev;
	Door* entry_point;
	Door* exit_point;
	Door* last_moved_through;
	Player* player1;
	Player* player2;
	bool is_current = false;
	std::string msg = "";

	GameRoom(Player* _player1, Player* _player2) : player1(_player1), player2(_player2) {}
	~GameRoom() {
		for (auto o : map_objects) delete o;
	}

	inline void addObject(MapObject* obj) {
		map_objects.push_back(obj);
		Door* door = dynamic_cast<Door*>(obj);
		if (door) {
			if (door->dest == DoorDest::NEXT)
				this->exit_point = door;
			else
				this->entry_point = door;
		}
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

	inline bool is_object_at_room(MapObject* val) {
		for (MapObject* obj : map_objects)
			if (obj == val) return true;
		return false;
	}

	int count_players() {
		int count_players = 0;
		for (MapObject* obj : map_objects) {
			Player* p = dynamic_cast<Player*>(obj);
			if (p != nullptr) count_players++;
		}
		return count_players;
	}
};