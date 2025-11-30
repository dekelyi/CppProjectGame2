#pragma once
#include <vector>
#include "Object.h"

class GameRoom;

class Collectible : public MapObject {
public:
	Collectible(V _pos, V _size, char _glyph) : MapObject(_pos, _size, _glyph) {};

	virtual bool handle_collision(GameRoom* room, MapObject* other, V dir) override;
};

class Collector : public virtual MapObject {
public:
	Collectible* collectible = nullptr;

	inline char getCollectibleGlyph() const {
		return (collectible != nullptr) ? collectible->getGlyph() : ' ';
	}

	void dump_collectible(GameRoom* room);

	virtual V getFreeNextPositon(GameRoom* room, std::vector<V> vnot);
};

class Key : public Collectible {
	//char glyph = 'K';
public:
	Key(V _pos, V _size) : Collectible(_pos, _size, (char)ObjType::KEY) {}
};