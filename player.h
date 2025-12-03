#pragma once
#include "Object.h"
#include "Collectible.h"
#include "prelude.h"


class Player : public MapObject {
	V direction = V(0, 0);

public:
	Collectible* collectible = nullptr;
	Player(char _glyph) : MapObject(V(1, 1), _glyph) { attr = A_FOREGROUND_CYAN; }
	~Player() {
		if (collectible) delete collectible;
	}

	virtual void handle_tick(GameRoom* room) override {
		if (this->direction != V(0,0) && !this->try_move(room, this->direction))
			this->direction = V(0, 0);
	}

	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, V dir) override {
		if (dir == this->direction) {
			return try_move(room, dir) ? CAN_MOVE : CANT_MOVE;
		}
		else return MapObject::handle_collision(room, other, dir);
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
		this->direction = this->get_moving_offset(e);
	}

	// Collectible stuff
	inline char getCollectibleGlyph() const {
		return (collectible != nullptr) ? collectible->getGlyph() : ' ';
	}

	void dump_collectible(GameRoom* room);

	V getDumpPosition(GameRoom* room);
};
