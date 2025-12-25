#pragma once
#include "Object.h"
#include "Collectible.h"
#include "prelude.h"


class Player : public MapObject {
public:
	Collectible* collectible = nullptr;

	Player(char _glyph) : MapObject(V(1, 1), _glyph) { attr = A_FOREGROUND_CYAN; }
	~Player() {
		if (collectible) delete collectible;
	}

	V get_moving_offset(Keypress e) {
		switch (e) {
			case Keypress::UP_1: case Keypress::UP_2: return V(0, -1);
			case Keypress::DOWN_1: case Keypress::DOWN_2: return V(0, 1);
			case Keypress::RIGHT_1: case Keypress::RIGHT_2: return V(1, 0);
			case Keypress::LEFT_1: case Keypress::LEFT_2: return V(-1, 0);
			default: return V(0, 0);
		}
	}

	virtual inline void move(const V& offset) override {
		MapObject::move(offset);
		if (collectible) collectible->move(offset);
	}

	inline void handle_movement(Keypress e) {
		V dir = this->get_moving_offset(e);
		for (auto& m: moves)
			if (m.kind == Move::EVENT && m.dir.is_same_direction(dir)) return;
		moves.remove_if([](const Move& m) { return m.kind == Move::KEYPRESS; });
		if (dir != V(0, 0))moves.push_back({ dir , USHRT_MAX, 1, Move::KEYPRESS });
	}

	virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override;
	virtual inline void handle_tick(GameRoom* room) override {
		MapObject::handle_tick(room);
		if (collectible) collectible->handle_tick(room);
	}

	// Collectible stuff
	inline char getCollectibleGlyph() const {
		return (collectible != nullptr) ? collectible->getGlyph() : ' ';
	}

	void dump_collectible(GameRoom* room);

	V getDumpPosition(GameRoom* room);
};
