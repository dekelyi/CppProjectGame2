#pragma once
#include "../Object.h"
#include "Collectible.h"

class Player : public MapObject {
public:
	static const unsigned DEFAULT_LIVES = 3;
	Collectible* collectible = nullptr;
	unsigned lives = DEFAULT_LIVES;
	unsigned score = 0;

	Player(char _glyph) : MapObject(V(1, 1), _glyph) { attr = A_FOREGROUND_CYAN; }
	~Player() { if (collectible) delete collectible; }

	V get_moving_offset(ConsoleMenu::Keypress e) {
		switch (e) {
			case ConsoleMenu::Keypress::UP_1: case ConsoleMenu::Keypress::UP_2: return V(0, -1);
			case ConsoleMenu::Keypress::DOWN_1: case ConsoleMenu::Keypress::DOWN_2: return V(0, 1);
			case ConsoleMenu::Keypress::RIGHT_1: case ConsoleMenu::Keypress::RIGHT_2: return V(1, 0);
			case ConsoleMenu::Keypress::LEFT_1: case ConsoleMenu::Keypress::LEFT_2: return V(-1, 0);
			default: return V(0, 0);
		}
	}

	virtual inline void move(const V& offset) override {
		MapObject::move(offset);
		if (collectible) collectible->move(offset);
	}

	inline void handle_movement(ConsoleMenu::Keypress e) {
		V dir = this->get_moving_offset(e);
		for (auto& m: moves)
			if (m.kind == Move::EVENT && m.dir.is_same_direction(dir)) return;
		moves.remove_if([](const Move& m) { return m.kind == Move::KEYPRESS; });
		if (dir != V(0, 0))moves.push_back({ dir , USHRT_MAX, 1, Move::KEYPRESS });
	}

	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override;

	virtual inline void handle_tick(GameRoom* room) override {
		if (collectible) collectible->handle_tick(room);
		MapObject::handle_tick(room);
	}

	inline char getCollectibleGlyph() const { return (collectible != nullptr) ? collectible->getGlyph() : ' '; }

	void dump_collectible(GameRoom* room);
	V getNextPosition(GameRoom* room);
};
