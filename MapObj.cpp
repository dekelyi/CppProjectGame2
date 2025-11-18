#include <iostream>
#include "MapObj.h"
#include "Screen.h"
#include "Map.h"
#include "Vector.h"

bool MapObject::can_move(Map* map, V dir) {
	V dest = pos + dir;
	if (dest.getX() < 0 || dest.getY() < 0
		|| dest.getX() >= 80 || dest.getY() >= 25)
		return false;

	auto obj = map->get_object_at(dest);
	if (obj)
		return obj->handle_collision(map, this, dir);
	else return true;
}

bool MapObject::try_move(Map* map, V dir) {
	if (dir != V(0, 0) && can_move(map, dir)) {
		map->clearObj(this);
		move(dir);
		map->drawObj(this);
		return true;
	}
	return false;
}