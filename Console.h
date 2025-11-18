#pragma once
#include "Vector.h"
#include "Object.h"
#include "keys.h"

enum class Mode {
	RUNNING,
	PAUSED,
	MENU
};

namespace ConsoleView {
	void init();
	void deinit();
	void drawAt(V pos, V size, const MapObject* glyph);
	void pause();
	Keypress get_keypress();
};