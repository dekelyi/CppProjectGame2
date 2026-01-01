#include "Riddle.h"

M_CODE Riddle::handle_collision(GameRoom* room, MapObject* obj, Move& move) {
	room->msg = new RiddleMsg(room, this);
	return CANT_MOVE;
}