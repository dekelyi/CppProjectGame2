#pragma once
#include "Object.h"
class Obstacle : public MapObject {
	char glyph = '*';
public:
	Obstacle(V _pos, V _size) : MapObject(_pos, _size, '*') {}

	virtual bool handle_collision(Game* game, MapObject* other, V dir) override {
		return try_move(game, dir);
	}
};

