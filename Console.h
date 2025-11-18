#pragma once
#include "Vector.h"
#include "prelude.h"

namespace ConsoleView {
	void drawAt(V pos, V size, char glyph);

	void init();
	void deinit();
	void pause();
	void menu();
	void manual();


	Keypress get_keypress();
};