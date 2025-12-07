#include "Object.h"
#include "GameView.h"
#include "Vector.h"

M_CODE MapObject::can_move(GameRoom* room, V dir) {
	V dest = pos + dir;
	// check if in bounds
	if (dest.is_out_of_bounds())
		return CANT_MOVE;

	auto obj = room->get_object_at(dest);
	if (obj)
		return obj->handle_collision(room, this, dir);
	else return CAN_MOVE;
}

bool MapObject::try_move(GameRoom* room, V dir) {
	if (dir == V(0, 0)) return false;
	M_CODE c = can_move(room, dir * speed);
	if (speed > 1) {
		short req_speed = (c == CAN_MOVE) ? speed : -1;
		for (int i = speed - 1; i >= 1; i--) {
			speed = i;
			c = can_move(room, dir * speed);
			if (c != CAN_MOVE) req_speed = -1;
			else if (req_speed == -1) req_speed = i;
		}
		if (req_speed == -1) return c;
		else speed = req_speed;
	}
	if (c == CAN_MOVE) {
		if (room->is_current && room->is_object_at_room(this)) this->clear();
		move(dir * speed);
		if (room->is_current && room->is_object_at_room(this)) this->draw();
		return true;
	}
	else if (c == MOVED) return true;
	return false;
}