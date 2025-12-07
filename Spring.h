#pragma once
#include "prelude.h"
#include "Vector.h"
#include "Object.h"
#include "Player.h"

class Spring : public MapObject {
public:
	S size;
	unsigned short compressed = 0;
	Player* force[N_MAX_PLAYERS] = { nullptr, nullptr };
	V compression_dir;

	Spring(V pos, S _size)
		: MapObject(pos, _size, (char)ObjType::SPRING), size(_size) {}

	virtual inline V getSize() const override {
		return (V)S(size.length - compressed, size.d);
	}

	virtual inline V getPosition() const override {
		return (compression_dir == V(1, 0) || compression_dir == V(0, 1)) ? (MapObject::getPosition() + compression_dir * compressed) : MapObject::getPosition();
	}

	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, V dir) override;
	virtual inline std::string getAttr() const override {
		return (compressed > 0) ? A_FOREGROUND_RED : A_FOREGROUND_GREEN;
	}

	inline void add_force(Player* p) {
		if (force[0] == nullptr) force[0] = p;
		else if (force[0] == p) return;
		else force[1] = p;
	}
};