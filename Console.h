#pragma once
#include "Vector.h"
#include "MapObj.h"

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
};