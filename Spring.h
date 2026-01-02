#pragma once
#include "prelude.h"
#include "Vector.h"
#include "Object.h"

class Spring : public MapObject {
	S size;
	unsigned short compressed = 0;
	MapObject* force = nullptr;
	V compression_dir = V(0,0);

	inline Move create_compressed_move() {
		return {
			V(-compression_dir.getX(), -compression_dir.getY()), // pos
			(unsigned short)(compressed * compressed), // duartion
			compressed, // speed
			Move::EVENT // kind
		};
	}
public:
	Spring(V pos, S _size)
		: MapObject(pos, _size, (char)ObjType::SPRING), size(_size) {}

	// Getters
	virtual inline V getSize() const override {
		return (V)S(size.length - compressed, size.d);
	}

	virtual inline V getPosition() const override {
		return (compression_dir == V(1, 0) || compression_dir == V(0, 1)) ? (MapObject::getPosition() + compression_dir * compressed) : MapObject::getPosition();
	}

	virtual inline const std::string getAttr() const override {
		return (compressed > 0) ? A_FOREGROUND_RED : A_FOREGROUND_GREEN;
	}

	// Events
	virtual bool handle_tick(GameRoom* room) override;
	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override;
};