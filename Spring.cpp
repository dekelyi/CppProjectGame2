#include "Spring.h"

M_CODE Spring::handle_collision(GameRoom* room, MapObject* other, V dir) {
	Player* p = dynamic_cast<Player*>(other);
	if (!size.is_same_direction(dir) || p == nullptr || (compression_dir != V(0,0) && compression_dir != dir)) return MapObject::handle_collision(room, other, dir);

	if (compression_dir == V(0, 0)) compression_dir = dir;
	this->clear();
	add_force(p);
	compressed++;

	if (compressed == size.length - 1) {
		p->speed = compressed;
		compressed = 0;
		force[0] = force[1] = nullptr;
		compression_dir = V(0, 0);
		p->direction = V(-p->direction.getX(), -p->direction.getY());
		p->try_move(room, p->direction);
		this->draw();
		return MOVED;
	}

	this->draw();
	return CAN_MOVE;
}