#include "GameView.h"
#include "Console.h"
#include "prelude.h"
#include <functional>
#include <iostream>

Mode pause_menu() {
	Mode mode = Mode::PAUSED;
	ConsoleView::pause(); // draw
	while (mode == Mode::PAUSED) {
		Keypress e = ConsoleView::get_keypress();
		if (e == Keypress::ESC) mode = Mode::CONTINUE;
		if (e == Keypress::H) mode = Mode::MENU;
	}
	return mode;
}

Mode start_menu() {
	Mode mode = Mode::MENU;
	ConsoleView::menu(); // draw
	while (mode == Mode::MENU) {
		Keypress e = ConsoleView::get_keypress();
		if (e == Keypress::_1) mode = Mode::RUNNING;
		if (e == Keypress::_7) { // disable/enable colors
			ConsoleView::colors = !ConsoleView::colors;
			ConsoleView::menu();
		}
		if (e == Keypress::_8) { // print manual
			ConsoleView::manual();
			ConsoleView::menu();
		}
		if (e == Keypress::_9 || e == Keypress::ESC) mode = Mode::EXIT;
	}
	return mode;
}

/**
* Display main menu runnig main game loop
*/
void _main(std::function<void(GameView*)> init) {
	Mode mode = Mode::MENU;
	GameView* game = nullptr;
	while ((bool)mode) {
		switch (mode) {
		case Mode::RUNNING:
			if (game) delete game;
			game = new GameView();
			try {
				init(game);
			}
			catch (const std::runtime_error& e) {
				ConsoleView::deinit();
				std::cout << e.what();
				return;
			}
			mode = game->run();
			break;
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
	if (game) delete game;
	ConsoleView::deinit();
}