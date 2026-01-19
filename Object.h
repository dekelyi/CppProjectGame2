#pragma once
#include <list>
#include "Vector.h"
#include "Console.h"

class GameRoom;

/**
 * MapObject - base class for all objects placed on the map.
 *
 * Responsibilities:
 * - Describe position, size and glyph used for rendering.
 * - Maintain queued moves and process movement logic.
 * - Provide hooks for collision handling and per-tick updates.
 */
class MapObject {
protected:
	friend class Spring;

	/**
	* A code representing the result of a moving operation
	*/
	enum class M_CODE {
		CAN_MOVE,
		CANT_MOVE,
		MOVED
	};

	/**
	* A move order
	*/
	struct Move {
		V dir;
		unsigned duration = 1,
			speed = 1;

		enum Kind { KEYPRESS, EVENT } kind;

		bool operator==(const Move& m) const = default;
	};

	V pos;
	V size;
	std::list<Move> moves;

	std::string attr = "";

public:
	char glyph;
	MapObject() : pos(V(-1,-1)), size(V(1, 1)), glyph(' ') {}
	MapObject(V _size, char _glyph) : pos(V(-1, -1)), size(_size), glyph(_glyph) {}
	MapObject(V _pos, V _size, char _glyph) : pos(_pos), size(_size), glyph(_glyph) {}

	// Setters and getters
	inline void setPosition(const V& _pos) { pos = _pos; }
	virtual inline V getPosition() const { return pos; }
	inline void setSize(const V& _size) { size = _size; }
	virtual inline V getSize() const { return size; }
	inline void setGlyph(const char _glyph) { glyph = _glyph; }
	inline char getGlyph() const { return glyph; }
	virtual inline const std::string getAttr() const { return attr; }
	virtual inline void move(const V& offset) { pos = pos + offset; }

	// is at a specific position
	inline bool is_at(const V& pos) const {
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

	/**
	* Try to move this object using `dir`. Movement may be split into
	* multiple steps if speed > 1. Returns true if movement succeeded.
	*/
	bool try_move(GameRoom* game, Move& dir);

	/**
	 * Check whether the object can move according to current room state.
	 * Returns an MapObject::M_CODE describing the result.
	 */
	M_CODE can_move(GameRoom* game, Move& move);

	/**
	 * Handle collision between this object and `obj` when attempting `move`.
	 * Default implementation allows movement only when queued moves match direction.
	 */
	virtual M_CODE handle_collision(GameRoom* room, MapObject* obj, Move& move);

	/** Per-tick update; advances active moves and removes expired ones. */
	virtual void handle_tick(GameRoom* room);
};