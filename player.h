#pragma once
#include "Object.h"
#include "Collectible.h"
#include "prelude.h"


class Player : public MapObject {
	friend class Spring; 
	Move* auto_movement = nullptr;

public:
	Collectible* collectible = nullptr;

	Player(char _glyph) : MapObject(V(1, 1), _glyph) { attr = A_FOREGROUND_CYAN; }
	~Player() {
		if (collectible) delete collectible;
	}

	V get_moving_offset(Keypress e) {
		switch (e) {
			case Keypress::UP_1: case Keypress::UP_2: return V(0, -1);
			case Keypress::DOWN_1: case Keypress::DOWN_2: return V(0, 1);
			case Keypress::RIGHT_1: case Keypress::RIGHT_2: return V(1, 0);
			case Keypress::LEFT_1: case Keypress::LEFT_2: return V(-1, 0);
			default: return V(0, 0);
		}
	}

	inline void handle_movement(Keypress e) {
		if (auto_movement) {
			moves.remove(*auto_movement);
			auto_movement = nullptr;
		}
		moves.push_back({ this->get_moving_offset(e) , USHRT_MAX, 1 });
		auto_movement = &moves.back();
	}

	// Collectible stuff
	inline char getCollectibleGlyph() const {
		return (collectible != nullptr) ? collectible->getGlyph() : ' ';
	}

	void dump_collectible(GameRoom* room);

	V getDumpPosition(GameRoom* room);
};
