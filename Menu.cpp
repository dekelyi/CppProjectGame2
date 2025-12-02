#include "GameView.h"
#include "Console.h"
#include "prelude.h"

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


void _main(void (*fn)(GameView*)) {
	Mode mode = Mode::MENU;
	while ((bool)mode) {
		if (mode == Mode::RUNNING) {
			GameView game;
			fn(&game);
			mode = game.run();
		}
		if (mode == Mode::PAUSED) mode = pause_menu();
		if (mode == Mode::MENU) mode = start_menu();
		if (mode == Mode::WINNING) {
			ConsoleView::won_game();
			while (ConsoleView::get_keypress() == Keypress::NONE);
			mode = start_menu();
		}
	}
	ConsoleView::deinit();
}