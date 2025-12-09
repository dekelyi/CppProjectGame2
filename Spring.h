#pragma once
#include "prelude.h"
#include "Vector.h"
#include "Object.h"
#include "Player.h"
#include <array>

class Spring : public MapObject {
	S size;
	unsigned short compressed = 0;
	std::array<Player*, N_MAX_PLAYERS> force;
	V compression_dir;
public:

	Spring(V pos, S _size)
		: MapObject(pos, _size, (char)ObjType::SPRING), size(_size) {}

	virtual inline V getSize() const override {
		return (V)S(size.length - compressed, size.d);
	}

	virtual inline V getPosition() const override {
		return (compression_dir == V(1, 0) || compression_dir == V(0, 1)) ? (MapObject::getPosition() + compression_dir * compressed) : MapObject::getPosition();
	}

	inline size_t count_force() const {
		size_t c = 0;
		for (auto p : force)
			if (p) c++;
		return c;
	}

	virtual inline void handle_tick(GameRoom* room) override {
		if (count_force() == 0) {
			if (compressed > 0) {
				this->clear();
				compressed--;
				this->draw();
			}
			else if (compression_dir != V(0,0))
				compression_dir = V(0, 0);
		}
		MapObject::handle_tick(room);
	}


	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override;
	virtual inline std::string getAttr() const override {
		return (compressed > 0) ? A_FOREGROUND_RED : A_FOREGROUND_GREEN;
	}

	inline void add_force(Player* p) {
		if (force[0] == nullptr) force[0] = p;
		else if (force[0] == p) return;
		else force[1] = p;
	}
};