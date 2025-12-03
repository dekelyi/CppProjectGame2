#include "Object.h"
#include "GameView.h"
#include "Vector.h"

M_CODE MapObject::can_move(GameRoom* room, V dir) {
	V dest = pos + dir;
	// check if in bounds
	if (dest.getX() < 0 || dest.getY() < 0
		|| dest.getX() >= SIZE_X || dest.getY() >= SIZE_Y)
		return CANT_MOVE;

	auto obj = room->get_object_at(dest);
	if (obj)
		return obj->handle_collision(room, this, dir);
	else return CAN_MOVE;
}

bool MapObject::try_move(GameRoom* room, V dir) {
	if (dir == V(0, 0)) return false;
	M_CODE c = can_move(room, dir);
	if (c == CAN_MOVE) {
		if (room->is_current && room->is_object_at_room(this)) this->clear();
		move(dir);
		if (room->is_current && room->is_object_at_room(this)) this->draw();
		return true;
	}
	else if (c == MOVED) return true;
	return false;
}