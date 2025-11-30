#pragma once
#include "Object.h"
#include "Collectible.h"
#include "prelude.h"

class Collector;

class Player : public virtual Collector, public virtual MapObject {
	V direction = V(0, 0);
public:
	Player(char _glyph) : MapObject(V(1, 1), _glyph) {}

	virtual void handle_tick(GameRoom* room) override {
		if (!this->try_move(room, this->direction)) this->direction = V(0, 0);
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

	virtual V getFreeNextPositon(GameRoom* room, std::vector<V> vnot) override {
		// Try to dump collectible behind the player
		V behind = this->getPosition() - this->direction;
		vnot.push_back(behind);
		return Collector::getFreeNextPositon(room, vnot);
	}
};
