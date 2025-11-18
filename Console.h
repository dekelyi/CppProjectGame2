#pragma once
#include "Vector.h"
#include "Object.h"
#include "keys.h"

enum class Mode {
	EXIT = 0,
	RUNNING,
	PAUSED,
	MENU,
};

namespace ConsoleView {
	void drawAt(V pos, V size, const MapObject* glyph);

	void init();
	void deinit();
	void pause();
	void menu();
	void manual();


	Keypress get_keypress();
};