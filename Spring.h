#pragma once
#include "prelude.h"
#include "Vector.h"
#include "Object.h"

class Spring : public MapObject {
	S size;
	unsigned short compressed = 0;
	MapObject* force = nullptr;
	V compression_dir = V(0,0);
public:

	Spring(V pos, S _size)
		: MapObject(pos, _size, (char)ObjType::SPRING), size(_size) {}

	virtual inline V getSize() const override {
		return (V)S(size.length - compressed, size.d);
	}

	virtual inline V getPosition() const override {
		return (compression_dir == V(1, 0) || compression_dir == V(0, 1)) ? (MapObject::getPosition() + compression_dir * compressed) : MapObject::getPosition();
	}

	inline Move&& create_new_move() {
		return {
			V(-compression_dir.getX(), -compression_dir.getY()), // pos
			(unsigned short)(compressed * compressed), // duartion
			compressed, // speed
			Move::EVENT // kind
		};
	}

	virtual inline void handle_tick(GameRoom* room) override {
		if (!force) {
			if (compressed > 0) {
				this->clear();
				compressed--;
				this->draw();
			}
			else if (compression_dir != V(0,0))
				compression_dir = V(0, 0);
		}
		else if (!force->is_moving_to(compression_dir)) {
			force->moves.remove_if([&](Move& m) { return m.dir.is_same_direction(compression_dir); });
			Move move = create_new_move();
			force->moves.push_back(move);
			force->try_move(room, move);
			force = nullptr;
		}
		MapObject::handle_tick(room);
	}


	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override;
	virtual inline std::string getAttr() const override {
		return (compressed > 0) ? A_FOREGROUND_RED : A_FOREGROUND_GREEN;
	}
};