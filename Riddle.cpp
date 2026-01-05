#include "Riddle.h"

void RiddleMsg::on_input(char ch) {
	int answer_index = ch - '1';
	if (answer_index >= 0 && answer_index < (int)riddle->data.answers.size()) {
		if ((size_t)answer_index == riddle->data.correct_answer_index) {
			Msg::text = "Correct answer!";
			if (player) player->score += 1;
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

M_CODE Riddle::handle_collision(GameRoom* room, MapObject* obj, Move& move) {
	room->msg = new RiddleMsg(room, this, dynamic_cast<Player*>(obj));
	return M_CODE::CANT_MOVE;
}
