#pragma once
#include "Object.h"
#include "player.h"
#include "Room.h"

class GameView {
	GameRoom* head = nullptr;
	GameRoom* last = nullptr;
	GameRoom* current = nullptr;
	size_t s = 0, i = 1;
	int msg_count = -1;

public:
	Player* player1;
	Player* player2;

	GameView() {
		player1 = new Player((char)ObjType::PLAYER_1);
		player2 = new Player((char)ObjType::PLAYER_2);
	}

	~GameView() {
		GameRoom* node = head, *tmp;
		while (node) {
			tmp = node->next;
			delete node;
			node = tmp;
		}

		delete player1;
		delete player2;
	}

	inline GameRoom* add_room() {
		auto room = new GameRoom(player1, player2);
		if (last) {
			room->prev = last;
			room->prev->next = room;
			last = room;
		}
		else {
			current = last = head = room;
			current->is_current = true;
		}
		s++;
		return room;
	}

	inline void advance_room() {
		current->is_current = false;
		if (current->next)
			current = current->next;
		current->is_current = true;
		i++;
	}

	inline void goback_room() {
		current->is_current = false;
		if (current->prev)
			current = current->prev;
		current->is_current = true;
		i--; // assume i>0
	}

	inline void drawAll() {
		for (MapObject* obj : current->map_objects)
			obj->draw();
	}

	Mode handle_keypress(Keypress e);
	Mode run();
	Mode check_room();
	void handle_tick() {
		for (MapObject* obj : current->map_objects)
			obj->handle_tick(current);
	}
	void drawHUD();
};