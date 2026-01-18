#include "GameView.h"
#include "player.h"
#include "Console.h"
#include "prelude.h"
#include <format>

using namespace std;

/** ------ Linked list stuff ------------*/

/**
 * Add a room to the game. Link into linked list and return pointer.
 */
GameRoom* GameView::add_room(const unsigned X, const unsigned Y, const unsigned legend_pos) {
	auto room = new GameRoom(X, Y, legend_pos, runner);
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

/** Move to the next room in the list and update index/flags. */
void GameView::advance_room() {
	current->is_current = false;
	if (current->next)
		current = current->next;
	current->is_current = true;
	i++;
}

/** Move to the previous room in the list and update index/flags. */
void GameView::goback_room() {
	current->is_current = false;
	if (current->prev)
		current = current->prev;
	current->is_current = true;
	i--; // assume i>0
}

/**
 * If current room has no players move forward/back based on last door used.
 * Returns Mode::WINNING when no players left in any room.
 */
Mode GameView::check_room() {
	if (current->p_players.count_players() == 0) {
		GameRoom* room = this->head;
		// check if we won thr game
		int count_rooms_with_players = 0;
		while (room) {
			if (room->p_players.count_players() > 0) count_rooms_with_players++;
			room = room->next;
		};
		if (count_rooms_with_players == 0) return Mode::WINNING;
		// else, move to the rooms
		if (current->p_doors.last_moved_through->dest == DoorDest::PREV)
			goback_room();
		else advance_room();
		// redraw room
		if (current != nullptr) {
			Console::init();
			draw();
		}
		else return Mode::WINNING;
	}
	return Mode::RUNNING;
}

/** Draw HUD lines with player inventory and room progress. */
void GameView::drawHUD(unsigned y) {
	Writer wr(V(5, y));
	wr.writeline(format(" PLAYER 1 ({}) INVERTORY: {} LIVES: {} SCORE: {}", this->player1->getGlyph(), this->player1->getCollectibleGlyph(), this->player1->lives, this->player1->score));
	wr.writeline(format(" PLAYER 2 ({}) INVERTORY: {} LIVES: {} SCORE: {}", this->player2->getGlyph(), this->player2->getCollectibleGlyph(), this->player2->lives, this->player2->score));

	wr = Writer(V(SIZE_X - 10, y));
	size_t nroom = this->i,
		lroom = this->s;
	wr.writeline(format("ROOM: {}/{}", nroom,lroom));
	wr.writeline(string(nroom, CH_BLOCK) + string(lroom-nroom, CH_BLOCK_GREY));
	Console::gotoxy(V(0, HUD_SPACE_TOP + y));
}

/** Display current room message modal until it expires. */
void GameView::drawMsg() {
	Console::init();
	string msg = this->current->msg->getText();
	Writer(V(5, 10)).writeline(msg);
	while (this->current->msg->is_active()) {
		Console::sleep(TICK);
		this->current->msg->handle_tick(*this);
		if (this->current->msg->getText() != msg) {
			Console::init();
			msg = this->current->msg->getText();
			Writer(V(5, 10)).writeline(msg);
		}
	}
}

/** Dispatch a keypress to players or menus; returns Mode when mode-switch is required. */
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

/** Main loop: tick, draw, input, sleep cycle until mode changes. */
Mode GameView::run() {
	Mode mode = Mode::RUNNING;
	Console::init();
	this->draw();
	while (mode == Mode::RUNNING) {
		this->handle_tick();
		if ((mode = this->check_room()) != Mode::RUNNING) return mode;
		if (this->current->msg->is_active()) {
			this->drawMsg();
			Console::init();
		}
		this->draw();
		Keypress e = runner->get_keypress();
		mode = runner->get_mode(this->handle_keypress(e));
		Console::sleep(TICK);
	}
	return mode;
}