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
	int msg_count = -1; // counter for msg display

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
	inline void drawAll() {
		// set current doors glyphs
		if (current->entry_point) current->entry_point->setGlyph('0' + i - 1);
		if (current->exit_point) current->exit_point->setGlyph('0' + i + 1);

		for (MapObject* obj : current->map_objects)
			obj->draw();
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