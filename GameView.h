#pragma once
#include <vector>
#include "Object.h"
#include "Console.h"
#include "player.h"
#include "Room.h"

class GameView {
	std::vector<GameRoom> rooms;
	size_t current_room_index = 0;

public:
	Player* player1;
	Player* player2;

	GameView() {
		player1 = new Player((char)ObjType::PLAYER_1);
		player2 = new Player((char)ObjType::PLAYER_2);
		add_room();
	}

	inline void add_room() {
		rooms.push_back(GameRoom(player1, player2));
	}

	inline GameRoom* current_room() {
		return &rooms[current_room_index];
	}

	inline void drawAll() {
		for (MapObject* obj : current_room()->map_objects)
			obj->draw();
	}

	inline void handle_tick() {
		for (MapObject* obj : current_room()->map_objects)
			obj->handle_tick(this->current_room());
	}

	Mode handle_keypress(Keypress e);
	Mode run();
	void drawHUD();
};