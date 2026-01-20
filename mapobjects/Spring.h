#pragma once
#include "ObjTypes.h"
#include "../Vector.h"
#include "../Object.h"
#include "../EventLogger.h"
#include <format>

/**
 * Spring object that compresses when an object stands on it and propels
 * objects in the opposite direction when released.
 */
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
	void handle_tick(GameRoom* room) override;
	M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override;
};

class SpringDecompressed : public Event {
public:
	const MapObject* target;
	unsigned val;
	SpringDecompressed(const MapObject* _actor, const MapObject* _target, unsigned _val) : Event(_actor), target(_target), val(_val) {}
	virtual string to_string() override {
		char t = target ? target->getGlyph() : ' ';
		return std::format("OBJ {} DECOMPRESSED BY {} VAL {}", actor->getGlyph(), t, val);
	}
};
