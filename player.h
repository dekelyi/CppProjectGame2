#pragma once
#include "Object.h"
#include "Collectible.h"

/**
 * Player class represents the entity controlled by the user.
 * Inherits from MapObject and can move around the game map, collect items, and interact with other game objects.
 */
class Player : public MapObject {
public:
	static const unsigned DEFAULT_LIVES = 3;	/**< Default number of lives for the player */
	Collectible* collectible = nullptr;	/**< Pointer to the collectible item the player currently holds */
	unsigned lives = DEFAULT_LIVES;	/**< Number of lives the player has */
	unsigned score = 0;	/**< Player's score */

	/**
	 * Create a player rendered with glyph `_glyph`.
	 */
	Player(char _glyph) : MapObject(V(1, 1), _glyph) { attr = A_FOREGROUND_CYAN; }
	
	/**
	 * Destroy the player object, freeing any held collectible.
	 */
	~Player() {
		if (collectible) delete collectible;
	}

	/**
	 * Convert a Keypress to a movement offset vector.
	 * @return V(0, 0) if no valid keypress is detected, otherwise
	 * returns a vector representing the direction and magnitude of the movement.
	 */
	V get_moving_offset(ConsoleMenu::Keypress e) {
		switch (e) {
			case ConsoleMenu::Keypress::UP_1: case ConsoleMenu::Keypress::UP_2: return V(0, -1);
			case ConsoleMenu::Keypress::DOWN_1: case ConsoleMenu::Keypress::DOWN_2: return V(0, 1);
			case ConsoleMenu::Keypress::RIGHT_1: case ConsoleMenu::Keypress::RIGHT_2: return V(1, 0);
			case ConsoleMenu::Keypress::LEFT_1: case ConsoleMenu::Keypress::LEFT_2: return V(-1, 0);
			default: return V(0, 0);
		}
	}

	/**
	 * Move the player and attached collectible (if any) by `offset`.
	 */
	virtual inline void move(const V& offset) override {
		MapObject::move(offset);
		if (collectible) collectible->move(offset);
	}

	/**
	 * Handle input-driven movement; enqueues a keypress move into the move queue.
	 */
	inline void handle_movement(ConsoleMenu::Keypress e) {
		V dir = this->get_moving_offset(e);
		for (auto& m: moves)
			if (m.kind == Move::EVENT && m.dir.is_same_direction(dir)) return;
		moves.remove_if([](const Move& m) { return m.kind == Move::KEYPRESS; });
		if (dir != V(0, 0))moves.push_back({ dir , USHRT_MAX, 1, Move::KEYPRESS });
	}

	/**
	 * Handle collision with another object.
	 * @param room The game room where the collision occurred.
	 * @param other Pointer to the other MapObject involved in the collision.
	 * @param move The move that resulted in the collision.
	 * @return M_CODE indicating the result of the collision handling.
	 */
	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override;

	/**
	 * Handle game tick events for the player.
	 * @param room The game room in which the tick event occurred.
	 */
	virtual inline void handle_tick(GameRoom* room) override {
		if (collectible) collectible->handle_tick(room);
		MapObject::handle_tick(room);
	}

	// Collectible stuff
	/**
	 * Get the glyph of the collectible item the player is holding.
	 * @return The glyph character of the collectible, or a space character if the player is not holding any collectible.
	 */
	inline char getCollectibleGlyph() const {
		return (collectible != nullptr) ? collectible->getGlyph() : ' ';
	}

	/**
	 * Drop currently-held collectible into the provided room at player's position.
	 * @param room The game room where the collectible will be dropped.
	 */
	void dump_collectible(GameRoom* room);

	/**
	 * Compute the next position for placing a player in the given room (used after room transition).
	 * @param room The game room for which to compute the next position.
	 * @return A vector representing the computed position.
	 */
	V getNextPosition(GameRoom* room);
};
