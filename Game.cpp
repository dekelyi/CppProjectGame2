#include "Game.h"
#include "player.h"
#include "Console.h"
#include "keys.h"
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

		handle_tick();
		Sleep(TICK);
	}
}

 Mode pause_menu() {
	 Mode mode = Mode::PAUSED;
	 ConsoleView::pause();
	 while (mode == Mode::PAUSED) {
		 Keypress e = ConsoleView::get_keypress();
		 if (e == Keypress::ESC) mode = Mode::RUNNING;
		 if (e == Keypress::DOWN_1) mode = Mode::MENU;
	 }
	 return mode;
 }

 Mode start_menu() {
	 Mode mode = Mode::MENU;
	 ConsoleView::menu();
	 while (mode == Mode::MENU) {
		 Keypress e = ConsoleView::get_keypress();
		 if (e == Keypress::_1) mode = Mode::RUNNING;
		 if (e == Keypress::_8) {
			 ConsoleView::manual();
			 ConsoleView::menu();
		 }
		 if (e == Keypress::_9 || e == Keypress::ESC) mode = Mode::EXIT;
	 }
	 return mode;
 }

 void Game::main() {
	 Mode mode = Mode::MENU;
	 while ((bool)mode) {
		 if (mode == Mode::RUNNING) mode = this->run();
		 if (mode == Mode::PAUSED) mode = pause_menu();
		 if (mode == Mode::MENU) mode = start_menu();
	 }
	 ConsoleView::deinit();
 }