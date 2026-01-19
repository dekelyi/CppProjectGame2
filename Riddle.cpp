#include "Riddle.h"
#include "Room.h"
#include "EventLogger.h"

void RiddleMsg::on_input(string str) {
	int answer_index;

	try {
		answer_index = stoi(str) - 1;
	}
	catch (...) {
		answer_index = -1;
	}

	if (answer_index >= 0 && answer_index < (int)riddle->data.answers.size()) {
		room->runner->handle_event(new EventRiddleInteracted(player, riddle, (unsigned)answer_index));
		if ((size_t)answer_index == riddle->data.correct_answer_index) {
			Msg::text = "Correct answer!";
			if (player) player->score += 1;
			room->runner->handle_event(new GainedScore(player, 1));
			room->clear((MapObject)*riddle);
			room->remove_object((MapObject*)riddle);
			riddle = nullptr;
		}
		else {
			Msg::text = "Wrong answer!";
		}
		ticks_left = 5;
		active = false;
	}
}

MapObject::M_CODE Riddle::handle_collision(GameRoom* room, MapObject* obj, Move& move) {
	room->msg = new RiddleMsg(room, this, dynamic_cast<Player*>(obj));
	return MapObject::M_CODE::CANT_MOVE;
}
