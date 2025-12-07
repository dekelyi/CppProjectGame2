#include "Object.h"
#include "GameView.h"
#include "Vector.h"

M_CODE MapObject::can_move(GameRoom* room, Move& move) {
	V dest = pos + move.dir * move.speed;
	// check if in bounds
	if (dest.is_out_of_bounds())
		return CANT_MOVE;

	auto obj = room->get_object_at(dest);
	if (obj)
		return obj->handle_collision(room, this, move);
	else return CAN_MOVE;
}

bool MapObject::try_move(GameRoom* room, Move& m) {
	if (m.dir == V(0, 0)) return false;
	M_CODE c = can_move(room, m);
	if (m.speed > 1) {
		short req_speed = (c == CAN_MOVE) ? m.speed : -1;
		for (int i = m.speed - 1; i >= 1; i--) {
			m.speed = i;
			c = can_move(room, m);
			if (c != CAN_MOVE) req_speed = -1;
			else if (req_speed == -1) req_speed = i;
		}
		if (req_speed == -1) return c;
		else m.speed = req_speed;
	}
	if (c == CAN_MOVE) {
		if (room->is_current && room->is_object_at_room(this)) this->clear();
		move(m.dir * m.speed);
		if (room->is_current && room->is_object_at_room(this)) this->draw();
		return true;
	}
	else if (c == MOVED) return true;
	return false;
}

void MapObject::handle_tick(GameRoom* room) {
	std::vector<Move> remove;
	for (Move& m : moves) {
		m.duartion--;
		if (m.duartion == 0 || !this->try_move(room, m)) {
			remove.push_back(m);
		}
	}
	for (const Move& m : remove) moves.remove(m);
}

M_CODE MapObject::handle_collision(GameRoom* room, MapObject* obj, Move& move) {
	bool can_move = false;
	for (auto& m : moves) 
		if (m.dir.is_same_direction(move.dir))
			can_move = can_move || try_move(room, m);
	return can_move ? CAN_MOVE : CANT_MOVE;
}