#include "GameView.h"
#include "player.h"
#include "Console.h"
#include "prelude.h"
#include <windows.h>
#include <format>

#define TICK 100

void GameView::drawHUD() {
	Writer wr(V(5, 1));
	wr.writeline(std::format(" PLAYER 1 ({}) INVERTORY: ", (char)ObjType::PLAYER_1));
	wr.writeline(std::format(" PLAYER 2 ({}) INVERTORY: ", (char)ObjType::PLAYER_2));
	ConsoleView::drawAt(V(0, HUD_SPACE_TOP - 1), V(SIZE_X, 1), ' ', UNDERSCORE, false);
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
			break;
		case Keypress::DISPOSE_2:
			break;
		case Keypress::ESC:
			return Mode::PAUSED;
		case Keypress::NONE:
		default:
			break;
	}
	return Mode::RUNNING;
}

 Mode GameView::run() {
	 Mode mode = Mode::RUNNING;
	ConsoleView::init();
	this->drawHUD();
	this->drawAll();
	while (mode == Mode::RUNNING) {
		Sleep(TICK);
		this->handle_tick();
		Keypress e = ConsoleView::get_keypress();
		mode = this->handle_keypress(e);
	}
	return mode;
}