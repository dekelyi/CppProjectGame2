#pragma once
#include "prelude.h"
#include "Object.h"
class Obstacle : public MapObject {
	//char glyph = '*';
public:
	Obstacle(V _pos, V _size) : MapObject(_pos, _size, (char)ObjType::OBSTCALE) {}

	virtual bool handle_collision(GameRoom* room, MapObject* other, V dir) override {
		return try_move(room, dir);
	}
};

