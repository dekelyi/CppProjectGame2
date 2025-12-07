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

	bool operator==(const Move& m) const = default;
};

class MapObject {
protected:
	V pos;
	V size;
	std::list<Move> moves;

	char glyph;
	std::string attr = "";

public:
	MapObject() : pos(V(-1,-1)), size(V(1, 1)), glyph(' ') {}
	MapObject(V _size, char _glyph) : pos(V(-1, -1)), size(_size), glyph(_glyph) {}
	MapObject(V _pos, V _size, char _glyph) : pos(_pos), size(_size), glyph(_glyph) {}

	inline void setPosition(V _pos) { pos = _pos; }
	virtual inline V getPosition() const { return pos; }
	inline void setSize(V _size) { size = _size; }
	virtual inline V getSize() const { return size; }
	inline void setGlyph(char _glyph) { glyph = _glyph; }
	inline char getGlyph() const { return glyph; }
	virtual inline std::string getAttr() const { return attr; }

	inline void draw() const { ConsoleView::drawAt(getPosition(), getSize(), glyph, ConsoleView::colors ? getAttr() : ""); }
	inline void clear() const { ConsoleView::drawAt(getPosition(), getSize(), ' '); }

	inline void move(V offset) { pos = pos + offset; }
	inline bool is_at(V pos) {
		return (
			pos.getX() >= getPosition().getX() &&
			pos.getY() >= getPosition().getY() &&
			pos.getX() < getPosition().getX() + getSize().getX() &&
			pos.getY() < getPosition().getY() + getSize().getY()
		);
	}

	bool try_move(GameRoom* game, Move& dir);
	M_CODE can_move(GameRoom* game, Move& move);
	virtual M_CODE handle_collision(GameRoom* room, MapObject* obj, Move& move);

	virtual void handle_tick(GameRoom* room);
};