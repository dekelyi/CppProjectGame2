#include "GameView.h"
#include "player.h"
#include "Console.h"
#include "prelude.h"
#include <windows.h>
#include <format>

using namespace std;

#define TICK 100

void GameView::drawHUD() {
	Writer wr(V(5, 1));
	wr.writeline(format(" PLAYER 1 ({}) INVERTORY: {}", this->player1->getGlyph(), this->player1->getCollectibleGlyph()));
	wr.writeline(format(" PLAYER 2 ({}) INVERTORY: {}", this->player2->getGlyph(), this->player2->getCollectibleGlyph()));

	wr = Writer(V(SIZE_X - 10, 1));
	size_t nroom = this->current_room_index + 1,
		lroom = this->rooms.size();
	wr.writeline(format("ROOM: {}/{}", nroom,lroom));
	wr.writeline(string(nroom, (char)178) + string(lroom-nroom, (char)176));
}

void drawBorders() {
	ConsoleView::drawAt(V(0, HUD_SPACE_TOP - 1), V(SIZE_X, 1), ' ', UNDERSCORE, false);
	ConsoleView::drawAt(V(0, HUD_SPACE_TOP + SIZE_Y), V(SIZE_X, 1), '-', "", false);
	ConsoleView::drawAt(V(SIZE_X, 0), V(1, SIZE_Y + HUD_SPACE_TOP), '|', "", false);
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
			this->player1->dump_collectible(this->current_room());
			break;
		case Keypress::DISPOSE_2:
			this->player2->dump_collectible(this->current_room());
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
	drawBorders();
	this->drawHUD();
	this->drawAll();
	while (mode == Mode::RUNNING) {
		Sleep(TICK);
		this->handle_tick();
		this->drawHUD();
		Keypress e = ConsoleView::get_keypress();
		mode = this->handle_keypress(e);
	}
	return mode;
}