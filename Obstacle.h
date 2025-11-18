#pragma once
#include "MapObj.h"
class Obstacle : public MapObject {
	char glyph = '*';
public:
	Obstacle(V _pos, V _size) : MapObject(_pos, _size, '*') {}
	virtual bool handle_collision(Map* map, MapObject* other, V dir) override {
		return try_move(map, dir);
	}
};

