#include <conio.h>
#include "Map.h"
#include "player.h"

#define ESC 27

void Map::init() {
	for (View* view : views) view->init();
}

 Mode Map::run() {
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
		 this->init();
		 mode = this->run();
		 if (mode == Mode::PAUSED) {
			 for (View* view : views) view->pause();
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
 }