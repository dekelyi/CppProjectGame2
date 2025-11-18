#include <conio.h>
#include "Map.h"
#include "player.h"
#include "Console.h"

#define ESC 27

 Mode Map::run() {
	ConsoleView::init();
	this->drawAll();
	while (true) {
		if (_kbhit()) {
			char ch = _getch();
			if (ch == ESC) return Mode::PAUSED;
			else player1.handle_input(this, ch);
		}
	}
}

 void Map::main() {
	 Mode mode = Mode::RUNNING;
	 while (mode == Mode::RUNNING) {
		 mode = this->run();
		 if (mode == Mode::PAUSED) {
			 ConsoleView::pause();
			 while (mode == Mode::PAUSED) {
				 if (_kbhit()) {
					 char ch = _getch();
					 if (ch == ESC) {
						 mode = Mode::RUNNING;
					 }
					 if (ch == 'x' || ch == 'X') mode = Mode::MENU;
				 }
			 }
		 }
	 }
	 ConsoleView::deinit();
 }