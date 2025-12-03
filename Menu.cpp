#include "GameView.h"
#include "Console.h"
#include "prelude.h"

Mode pause_menu() {
	Mode mode = Mode::PAUSED;
	ConsoleView::pause();
	while (mode == Mode::PAUSED) {
		Keypress e = ConsoleView::get_keypress();
		if (e == Keypress::ESC) mode = Mode::CONTINUE;
		if (e == Keypress::H) mode = Mode::MENU;
	}
	return mode;
}

Mode start_menu() {
	Mode mode = Mode::MENU;
	ConsoleView::menu();
	while (mode == Mode::MENU) {
		Keypress e = ConsoleView::get_keypress();
		if (e == Keypress::_1) mode = Mode::RUNNING;
		if (e == Keypress::_7) {
			ConsoleView::colors = !ConsoleView::colors;
			ConsoleView::menu();
		}
		if (e == Keypress::_8) {
			ConsoleView::manual();
			ConsoleView::menu();
		}
		if (e == Keypress::_9 || e == Keypress::ESC) mode = Mode::EXIT;
	}
	return mode;
}


void _main(void (*init)(GameView*)) {
	Mode mode = Mode::MENU;
	GameView* game = nullptr;
	while ((bool)mode) {
		switch (mode) {
		case Mode::RUNNING:
			game = new GameView();
			init(game);
		case Mode::CONTINUE:
			mode = game->run();
			break;
		case Mode::PAUSED:
			mode = pause_menu();
			break;
		case Mode::MENU:
			mode = start_menu();
			break;
		case Mode::WINNING:
			ConsoleView::won_game();
			while (ConsoleView::get_keypress() == Keypress::NONE);
			mode = Mode::MENU;
			break;
		default:
			mode = Mode::EXIT;
			break;
		}
	}
	ConsoleView::deinit();
}