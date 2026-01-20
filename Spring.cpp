#include "Spring.h"
#include "Player.h"
#include "Room.h"
#include "EventLogger.h"

MapObject::M_CODE Spring::handle_collision(GameRoom* room, MapObject* other, Move& move) {
	if (
		!size.is_same_direction(move.dir) || // same axis 
		(compression_dir != V(0,0) && compression_dir != move.dir) || // same direction
		!dynamic_cast<Player*>(other) // a player
		) return MapObject::handle_collision(room, other, move);

	if (compression_dir == V(0, 0)) compression_dir = move.dir;
	room->clear(*this);
	force = other;

	compressed++;
	if (compressed == size.length) {
		move = create_compressed_move();
		force = nullptr;
		return MapObject::M_CODE::CAN_MOVE;
	}

	return MapObject::M_CODE::CAN_MOVE;
}

void Spring::handle_tick(GameRoom* room) {
	MapObject::handle_tick(room);
	if (!force) {
		if (compressed > 0) {
			room->clear(*this);
			compressed--;
		}
		else if (compression_dir != V(0, 0))
			compression_dir = V(0, 0);
	}
	else if (!force->is_moving_to(compression_dir)) {
		force->moves.remove_if([&](Move& m) { return m.dir.is_same_direction(compression_dir); });
		Move move = create_compressed_move();
		force->moves.push_back(move);
		force->try_move(room, move);
		// emit decompressed event for the launched object
		room->runner->handle_event(new SpringDecompressed(this, force, compressed));
		force = nullptr;
	}
}