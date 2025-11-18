#pragma once
#include "MapObj.h"
#include "keys.h"

class Player : public MapObject {

	public:
		V direction = V(0, 0);
	Player(char _glyph) : MapObject(V(1, 1), _glyph) {}
	Player() : MapObject(V(1, 1), '@') {}

	static V get_moving_offset(Keypress e) {
		switch (e) {
		case Keypress::UP_1: case Keypress::UP_2: return V(0, -1);
		case Keypress::DOWN_1: case Keypress::DOWN_2: return V(0, 1);
		case Keypress::RIGHT_1: case Keypress::RIGHT_2: return V(1, 0);
		case Keypress::LEFT_1: case Keypress::LEFT_2: return V(-1, 0);
		default: return V(0, 0);
		}
	}
};
