#pragma once
#include "ObjTypes.h"
#include "Object.h"

/**
 * Obstacle - simple blocking object. Blocks movement unless underlying
 * try_move logic allows it.
 */
class Obstacle : public MapObject {
public:
	Obstacle(V _pos, V _size) : MapObject(_pos, _size, (char)ObjType::OBSTCALE) { attr = A_FOREGROUND_YELLOW;  }

	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override {
		return try_move(room, move) ? M_CODE::CAN_MOVE : M_CODE::CANT_MOVE;
	}
};

