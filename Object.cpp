#include "Object.h"
#include "Game.h"
#include "Vector.h"

bool MapObject::can_move(Game* game, V dir) {
	V dest = pos + dir;
	// check if in bounds
	if (dest.getX() < 0 || dest.getY() < 0
		|| dest.getX() >= 80 || dest.getY() >= 25)
		return false;

	auto obj = game->get_object_at(dest);
	if (obj)
		return obj->handle_collision(game, this, dir);
	else return true;
}

bool MapObject::try_move(Game* game, V dir) {
	if (dir == V(0, 0)) return false;
	if (can_move(game, dir)) {
		game->clearObj(this);
		move(dir);
		game->drawObj(this);
		return true;
	}
	return false;
}