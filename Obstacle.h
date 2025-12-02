#pragma once
#include "prelude.h"
#include "Object.h"
class Obstacle : public MapObject {
public:
	Obstacle(V _pos, V _size) : MapObject(_pos, _size, (char)ObjType::OBSTCALE) { attr = A_FOREGROUND_YELLOW;  }

	virtual bool handle_collision(GameRoom* room, MapObject* other, V dir) override {
		return try_move(room, dir);
	}
};

