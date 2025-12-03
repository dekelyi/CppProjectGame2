#pragma once
#include <vector>
#include "Object.h"

class GameRoom;

class Collectible : public MapObject {
public:
	Collectible(V _pos, V _size, char _glyph) : MapObject(_pos, _size, _glyph) { attr = A_FOREGROUND_MAGENTA;  };

	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, V dir) override;
};

class Key : public Collectible {
public:
	Key(V _pos, V _size) : Collectible(_pos, _size, (char)ObjType::KEY) {}
};