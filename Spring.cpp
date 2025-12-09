#include "Spring.h"

M_CODE Spring::handle_collision(GameRoom* room, MapObject* other, Move& move) {
	Player* p = dynamic_cast<Player*>(other);
	if (!size.is_same_direction(move.dir) || p == nullptr || (compression_dir != V(0,0) && compression_dir != move.dir)) return MapObject::handle_collision(room, other, move);

	if (compression_dir == V(0, 0)) compression_dir = move.dir;
	this->clear();
	add_force(p);
	compressed++;

	if (compressed == size.length) {
		Move new_move = {
			V(-move.dir.getX(), -move.dir.getY()), // pos
			compressed * compressed, // duartion
			compressed * count_force(), // speed
			Move::EVENT // kind
		};
		move.duartion = 0;
		p->moves.push_back(new_move);

		compressed--;
		force[0] = force[1] = nullptr;
		this->draw();
		return MOVED;
	}

	this->draw();
	return CAN_MOVE;
}