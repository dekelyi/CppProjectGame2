#include "GameView.h"
#include "player.h"
#include "Console.h"
#include "prelude.h"
#include <format>

using namespace std;

/** ------ Linked list stuff ------------*/

/**
Add a room to the game
*/
GameRoom* GameView::add_room() {
	auto room = new GameRoom(player1, player2);
	if (last) {
		room->prev = last;
		room->prev->next = room;
		last = room;
	}
	else {
		current = last = head = room;
		current->is_current = true;
	}
	s++;
	return room;
}

/**
Move to the next room
*/
void GameView::advance_room() {
	current->is_current = false;
	if (current->next)
		current = current->next;
	current->is_current = true;
	i++;
}

/**
Move to the previous room
*/
void GameView::goback_room() {
	current->is_current = false;
	if (current->prev)
		current = current->prev;
	current->is_current = true;
	i--; // assume i>0
}

/**
Checks if no players are in the current room
if so, move to the room where the last player is at
*/
Mode GameView::check_room() {
	if (current->count_players() == 0) {
		GameRoom* room = this->head;
		// check if we won thr game
		int count_rooms_with_players = 0;
		while (room) {
			if (room->count_players() > 0) count_rooms_with_players++;
			room = room->next;
		};
		if (count_rooms_with_players == 0) return Mode::WINNING;
		// else, move to the rooms
		if (current->last_moved_through->dest == DoorDest::PREV)
			goback_room();
		else advance_room();
		// redraw room
		if (current != nullptr) {
			ConsoleView::init();
			ConsoleView::draw_borders();
			drawAll();
		}
		else return Mode::WINNING;
	}
	return Mode::RUNNING;
}

/**
Draws the HUD of the current game
*/
void GameView::drawHUD() {
	Writer wr(V(5, 1));
	wr.writeline(format(" PLAYER 1 ({}) INVERTORY: {}", this->player1->getGlyph(), this->player1->getCollectibleGlyph()));
	wr.writeline(format(" PLAYER 2 ({}) INVERTORY: {}", this->player2->getGlyph(), this->player2->getCollectibleGlyph()));

	// Message ticket
	if (!this->current->msg.empty()) {
		if (msg_count == 0) { // clear msg
			wr.writeline(std::string(this->current->msg.size(), ' '));
			this->current->msg = "";
			msg_count = -1;
		}
		else { // write msg
			wr.writeline(this->current->msg);
			if (msg_count == -1) msg_count = MSG_TICKS;
			else msg_count--;
		}
	}

	wr = Writer(V(SIZE_X - 10, 1));
	size_t nroom = this->i,
		lroom = this->s;
	wr.writeline(format("ROOM: {}/{}", nroom,lroom));
	wr.writeline(string(nroom, (char)178) + string(lroom-nroom, (char)176));
}



Mode GameView::handle_keypress(Keypress e) {
	switch (e) {
		case Keypress::UP_1:
		case Keypress::DOWN_1:
		case Keypress::LEFT_1:
		case Keypress::RIGHT_1:
		case Keypress::STAY_1:
			this->player1->handle_movement(e);
			break;
		case Keypress::UP_2:
		case Keypress::DOWN_2:
		case Keypress::LEFT_2:
		case Keypress::RIGHT_2:
		case Keypress::STAY_2:
			this->player2->handle_movement(e);
			break;
		case Keypress::DISPOSE_1:
			this->player1->dump_collectible(this->current);
			break;
		case Keypress::DISPOSE_2:
			this->player2->dump_collectible(this->current);
			break;
		case Keypress::ESC:
			return Mode::PAUSED;
		case Keypress::NONE:
		default:
			break;
	}
	return Mode::RUNNING;
}

/**
Main game loop
*/
Mode GameView::run() {
	Mode mode = Mode::RUNNING;
	ConsoleView::init();
	ConsoleView::draw_borders();
	this->drawHUD();
	this->drawAll();
	while (mode == Mode::RUNNING) {
		console_sleep(TICK);
		this->handle_tick();
		if ((mode = this->check_room()) != Mode::RUNNING) return mode;
		this->drawHUD();
		Keypress e = ConsoleView::get_keypress();
		mode = this->handle_keypress(e);
	}
	return mode;
}