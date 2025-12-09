#include "Spring.h"
#include "Player.h"

M_CODE Spring::handle_collision(GameRoom* room, MapObject* other, Move& move) {
	if (
		!size.is_same_direction(move.dir) || // same axis 
		(compression_dir != V(0,0) && compression_dir != move.dir) || // same direction
		!dynamic_cast<Player*>(other) // a player
		) return MapObject::handle_collision(room, other, move);

	if (compression_dir == V(0, 0)) compression_dir = move.dir;
	this->clear();
	force = other;

	compressed++;
	if (compressed == size.length) {
		move = create_new_move();
		force = nullptr;
		this->draw();
		return CAN_MOVE;
	}

	this->draw();
	return CAN_MOVE;
}