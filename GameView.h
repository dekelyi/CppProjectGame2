#pragma once
#include "Object.h"
#include "player.h"
#include "Room.h"

/**
* Object representing a game
* Containing a linked list of rooms
* 
* Does all the Event handling
*/
class GameView {
	// linked list of rooms
	GameRoom* head = nullptr;
	GameRoom* last = nullptr;
	unsigned char s = 0, // total size
		i = 1; // current room

public:
	GameRoom* current = nullptr; // current room
	Player* player1;
	Player* player2;

	inline GameView() {
		player1 = new Player((char)ObjType::PLAYER_1);
		player2 = new Player((char)ObjType::PLAYER_2);
	}

	~GameView() {
		// delete linked list
		GameRoom* node = head, *tmp;
		while (node) {
			tmp = node->next;
			delete node;
			node = tmp;
		}

		delete player1;
		delete player2;
	}

	inline void init_rooms() {
		unsigned int idx = 1;
		GameRoom* node = head;
		while (node) {
			node->init(idx++);
			node = node->next;
		}
		current->add_object(player1);
		current->add_object(player2);
	}

	/**
	Do one tick of the game
	*/
	inline void handle_tick() {
		for (MapObject* obj : current->map_objects)
			obj->handle_tick(current);
	}

	/**
	Draw all objects
	*/
	inline void drawAll(bool clear = false) {
		for (MapObject* obj : current->map_objects)
			clear ? current->clear(*obj) : current->draw(*obj);
	}
	/**
	Add a room to the game
	*/
	GameRoom* add_room();
	/**
	Move to the next room
	*/
	void advance_room();
	/**
	Move to the previous room
	*/
	void goback_room();

	/**
	Draws the HUD of the current game
	*/
	void drawHUD();

	void drawMsg();
	/**
	Checks if no players are in the current room
	if so, move to the room where the last player is at
	*/
	Mode check_room();
	Mode handle_keypress(Keypress e);
	/**
	Main game loop
	*/
	Mode run();
};