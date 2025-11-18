#include <conio.h>
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
		if (e == Keypress::NONE);
		else if (e == Keypress::ESC) return Mode::PAUSED;
		else if (this->player1 && (e == Keypress::UP_1 || e == Keypress::DOWN_1 || e == Keypress::LEFT_1 || e == Keypress::RIGHT_1)) this->player1->direction = Player::get_moving_offset(e);
		else if (this->player2 && (e == Keypress::UP_2 || e == Keypress::DOWN_2 || e == Keypress::LEFT_2 || e == Keypress::RIGHT_2)) this->player2->direction = Player::get_moving_offset(e);

		handle_tick();
		Sleep(TICK);
	}
}

 void Game::main() {
	 Mode mode = Mode::RUNNING;
	 while (mode == Mode::RUNNING) {
		 mode = this->run();
		 if (mode == Mode::PAUSED) {
			 ConsoleView::pause();
			 while (mode == Mode::PAUSED) {
				 Keypress e = ConsoleView::get_keypress();
				 if (e == Keypress::ESC) mode = Mode::RUNNING;
				 if (e == Keypress::DOWN_1) mode = Mode::MENU;
			 }
		 }
	 }
	 ConsoleView::deinit();
 }