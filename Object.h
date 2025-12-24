#pragma once
#include <list>
#include "Vector.h"
#include "Console.h"

class GameRoom;

enum M_CODE {
	CAN_MOVE,
	CANT_MOVE,
	MOVED
};

struct Move {
	V dir;
	unsigned short duartion = 1,
		speed  = 1;

	enum Kind {KEYPRESS, EVENT} kind;

	bool operator==(const Move& m) const = default;
};

struct Draw {
	V pos;
	V size;
	char glyph;
	std::string attr = "";
};

class MapObject {
protected:
	friend class Spring;

	V pos;
	V size;
	std::list<Move> moves;

	char glyph;
	std::string attr = "";

public:
	MapObject() : pos(V(-1,-1)), size(V(1, 1)), glyph(' ') {}
	MapObject(V _size, char _glyph) : pos(V(-1, -1)), size(_size), glyph(_glyph) {}
	MapObject(V _pos, V _size, char _glyph) : pos(_pos), size(_size), glyph(_glyph) {}

	// Setters and getts
	inline void setPosition(const V& _pos) { pos = _pos; }
	virtual inline V getPosition() const { return pos; }
	inline void setSize(const V& _size) { size = _size; }
	virtual inline V getSize() const { return size; }
	inline void setGlyph(const char _glyph) { glyph = _glyph; }
	inline char getGlyph() const { return glyph; }
	virtual inline const std::string getAttr() const { return attr; }
	inline void move(const V& offset) { pos = pos + offset; }

	// is at a specific position
	inline bool is_at(const V& pos) {
		return (
			pos.getX() >= getPosition().getX() &&
			pos.getY() >= getPosition().getY() &&
			pos.getX() < getPosition().getX() + getSize().getX() &&
			pos.getY() < getPosition().getY() + getSize().getY()
		);
	}

	// is in direction of moving
	inline bool is_moving_to(const V& pos) const {
		for (const Move& move : moves)
			if ((move.dir * pos) > 0)
				return true;
		return false;
	}

	bool try_move(GameRoom* game, Move& dir);
	M_CODE can_move(GameRoom* game, Move& move);
	virtual M_CODE handle_collision(GameRoom* room, MapObject* obj, Move& move);

	virtual void handle_tick(GameRoom* room);
};