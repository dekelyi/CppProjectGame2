#pragma once
#include <ctype.h>
#include "MapObj.h"
#include "Map.h"

class Player : public MapObject {
	const char* keys; // array of 4 chars representing up, down, left, right controls

	V get_moving_offset(char input) {
		if (input == keys[0]) return V(0, -1); // up
		if (input == keys[1]) return V(0, 1);  // down
		if (input == keys[2]) return V(-1, 0); // left
		if (input == keys[3]) return V(1, 0);  // right
		return V(0, 0); // no movement
	}

	inline bool can_move(Map* map, V dir) {
		auto obj = map->get_object_at(pos + dir);
		return obj ? obj->handle_collision(this) : true;
	}

	public:
	Player(char _glyph, char* _keys) : MapObject(V(1, 1), _glyph), keys(_keys) {}

	bool handle_input(Map* map, char input) {
		V dir = get_moving_offset(toupper(input));
		if (dir != V(0, 0) && can_move(map, dir)) {
			map->clearObj(this);
			move(dir);
			map->drawObj(this);
			return true;
		}
		return false;
	}
};

extern Player player1;