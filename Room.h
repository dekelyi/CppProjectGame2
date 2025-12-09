#pragma once
#include <vector>
#include "Object.h"
#include "player.h"
#include "Door.h"

class Door;

/** A single game room */
class GameRoom {
	friend class GameView;
	std::vector<MapObject*> map_objects; // all objects currently in this room

public:
	// linked lsit
	GameRoom* next = nullptr;
	GameRoom* prev = nullptr;
	// move-between-doors-stuff
	Door* entry_point = nullptr;
	Door* exit_point = nullptr;
	Door* last_moved_through = nullptr;

	Player* player1 = nullptr;
	Player* player2 = nullptr;

	bool is_current = false;
	std::string msg = "";

	GameRoom(Player* _player1, Player* _player2) : player1(_player1), player2(_player2) {}
	inline ~GameRoom() {
		for (auto o : map_objects) if (o != player1 && o != player2) delete o;
	}

	inline void add_object(MapObject* obj) {
		map_objects.push_back(obj);
		Door* door = dynamic_cast<Door*>(obj);
		if (door) {
			if (door->dest == DoorDest::NEXT)
				this->exit_point = door;
			else
				this->entry_point = door;
		}
	}

	inline void remove_object(MapObject* obj) {
		map_objects.erase(std::remove(map_objects.begin(), map_objects.end(), obj));
	}

	inline MapObject* get_object_at(V pos) {
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

	inline int count_players() {
		int count_players = 0;
		for (MapObject* obj : map_objects) {
			Player* p = dynamic_cast<Player*>(obj);
			if (p != nullptr) count_players++;
		}
		return count_players;
	}
};