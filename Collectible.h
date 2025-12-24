#pragma once
#include "Object.h"

class GameRoom;

class Collectible : public MapObject {

public:
	Collectible(V _pos, V _size, char _glyph) : MapObject(_pos, _size, _glyph) { attr = A_FOREGROUND_MAGENTA;  };

	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override;
	virtual void handle_collector_movement(GameRoom* room, V old_pos, V new_pos) {};
};

class Key : public Collectible {
public:
	Key(V _pos, V _size) : Collectible(_pos, _size, (char)ObjType::KEY) {}
};

class Torch : public Collectible {
public:
	unsigned short area;
	Torch(V _pos, unsigned short _area) : Collectible(_pos, V(1,1), (char)ObjType::TORCH), area(_area) {}

	virtual void handle_collector_movement(GameRoom* room, V old_pos, V new_pos) override;
};