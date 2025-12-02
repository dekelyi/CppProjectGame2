#pragma once
#include <vector>
#include "Object.h"
#include "Console.h"
#include "player.h"
#include "Room.h"

class GameView {
	std::vector<GameRoom*> rooms;
	size_t current_room_index = 0;

public:
	Player* player1;
	Player* player2;

	GameView() {
		player1 = new Player((char)ObjType::PLAYER_1);
		player2 = new Player((char)ObjType::PLAYER_2);
		add_room();
	}

	inline GameRoom* add_room() {
		auto room = new GameRoom(player1, player2);
		if (rooms.size() > 0) {
			room->prev = rooms[rooms.size() - 1];
			room->prev->next = room;
		}
		rooms.push_back(room);
		return room;
	}

	inline GameRoom* current_room() {
		return (current_room_index < rooms.size()) ? rooms[current_room_index] : nullptr;
	}

	inline void drawAll() {
		for (MapObject* obj : current_room()->map_objects)
			obj->draw();
	}

	Mode handle_keypress(Keypress e);
	Mode run();
	Mode check_room();
	void handle_tick() {
		for (MapObject* obj : current_room()->map_objects)
			obj->handle_tick(current_room());
	}
	void drawHUD();
};