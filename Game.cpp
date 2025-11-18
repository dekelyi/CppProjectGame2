#include "Game.h"
#include "player.h"
#include "Console.h"
#include "prelude.h"
#include <windows.h>

#define TICK 100

 Mode Game::run() {
	ConsoleView::init();
	this->drawAll();
	while (true) {
		Keypress e = ConsoleView::get_keypress();
		switch (e) {
		case Keypress::UP_1:
		case Keypress::DOWN_1:
		case Keypress::LEFT_1:
		case Keypress::RIGHT_1:
		case Keypress::STAY_1:
			this->player1->direction = Player::get_moving_offset(e);
			break;
		case Keypress::UP_2:
		case Keypress::DOWN_2:
		case Keypress::LEFT_2:
		case Keypress::RIGHT_2:
		case Keypress::STAY_2:
			this->player2->direction = Player::get_moving_offset(e);
			break;
		case Keypress::DISPOSE_1:
			break;
		case Keypress::DISPOSE_2:
			break;
		case Keypress::ESC:
			return Mode::PAUSED;
		case Keypress::NONE:
		default:
			break;
		}

		this->handle_tick();
		Sleep(TICK);
	}
}