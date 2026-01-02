#include "Object.h"
#include "GameView.h"
#include "Vector.h"

/**
Checks if the object can move in a specific way, considering other objects in the way
*/
M_CODE MapObject::can_move(GameRoom* room, Move& move) {
	V dest = pos + move.dir;
	// check if in bounds
	if (dest.is_out_of_bounds())
		return CANT_MOVE;

	auto obj = room->get_object_at(dest);
	if (obj)
		return obj->handle_collision(room, this, move);
	else return CAN_MOVE;
}

/**
Try to move in a specific way and draws the outcome
*/
bool MapObject::try_move(GameRoom* room, Move& m) {
	if (m.dir == V(0, 0)) return false;
	if (m.speed > 1) {
		bool r = true;
		int speed = m.speed, i = 1;
		m.speed = 1;
		for (; r && i <= speed; i++) {
			r = r && try_move(room, m);
		}
		m.speed = i - 1;
		return (m.speed) ? true : false;
	}
	M_CODE c = can_move(room, m);
	if (c == CAN_MOVE) {
		room->clear(*this);
		move(m.dir * m.speed);
		return true;
	}
	else if (c == MOVED) return true;
	return false;
}

/**
Move one iteration each tick
*/
void MapObject::handle_tick(GameRoom* room) {
	std::vector<Move> remove;
	for (Move& m : moves) {
		m.duartion--;
		if (m.duartion == 0 || !this->try_move(room, m) || m.duartion == 0) {
			remove.push_back(m);
		}
	}
	for (const Move& m : remove) moves.remove(m);
}

M_CODE MapObject::handle_collision(GameRoom* room, MapObject* obj, Move& move) {
	bool can_move = false;
	for (auto& m : moves)
		if (m.dir.is_same_direction(move.dir))
			//can_move = can_move || try_move(room, m);
			can_move = true;
	return can_move ? CAN_MOVE : CANT_MOVE;
}