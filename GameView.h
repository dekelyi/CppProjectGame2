#pragma once
#include "Object.h"
#include "player.h"
#include "Room.h"
#include "GameRunner.h"

/**
 * GameView - application-level game container
 *
 * Responsibilities:
 * - Owns the linked list of GameRoom instances and players.
 * - Drives the main game loop and event routing.
 * - Provides room management helpers (add, advance, go back).
 *
 * Public methods are documented inline.
 */
class GameView {
	// linked list of rooms
	GameRoom* head = nullptr;
	GameRoom* last = nullptr;
	unsigned char s = 0, // total size
		i = 1; // current room

public:
	GameRunner* runner = nullptr;
	GameRoom* current = nullptr; // current room
	Player* player1;
	Player* player2;

	/**
	 * Construct a GameView and create two player instances.
	 */
	inline GameView(GameRunner* _runner) : runner(_runner) {
		if (!runner) runner = new KeyboardGameRunner();
		runner->init();
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

		runner->deinit();
	}

	/**
	 * Initialize all rooms (call init on each GameRoom and its props).
	 */
	inline void init_rooms() {
		unsigned int idx = 1;
		GameRoom* node = head;
		while (node) {
			node->init(idx++);
			node = node->next;
		}
		//current->add_object(player1);
		//current->add_object(player2);
	}

	/**
	 * Perform one game tick by delegating tick handling to map objects.
	 */
	inline void handle_tick() {
		for (MapObject* obj : current->map_objects)
			obj->handle_tick(current);
	}

	/**
	 * Render the current room by drawing each MapObject and then the HUD.
	 */
	inline void draw() {
		if (!runner->should_draw_screen()) return;
		for (MapObject* obj : current->map_objects)
			current->draw(*obj);
		current->drawBuffer([&](unsigned y) { drawHUD(y); });
	}

	/**
	 * Add a new room to the game linked list.
	 * @param X Width of the room in cells (default SIZE_X).
	 * @param Y Height of the room in cells (default SIZE_Y).
	 * @param legend_pos Y position of the legend line.
	 * @returns pointer to the newly created GameRoom.
	 */
	GameRoom* add_room(const unsigned X = SIZE_X, const unsigned Y = SIZE_Y, const unsigned legend_pos = 0);

	/** Move to the next room in the linked list (if any). */
	void advance_room();

	/** Move to the previous room in the linked list (if any). */
	void goback_room();

	/** Draw the HUD for the current game state at vertical offset `y`. */
	void drawHUD(unsigned y);

	/** Draw active message modal (blocks until message inactive). */
	void drawMsg();

	/**
	 * Check whether players exist in the current room; if none, move to a room
	 * where players remain. Returns the Mode to switch to (RUNNING, WINNING, etc).
	 */
	Mode check_room();

	/** Handle a Keypress event and update players/room accordingly; returns selected Mode. */
	Mode handle_keypress(Keypress e);

	/**
	 * Main game loop. Runs until a Mode other than RUNNING is returned.
	 * Returns the Mode which the caller should handle next.
	 */
	Mode run();
};