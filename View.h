#pragma once
#include "Vector.h"
#include "MapObj.h"

enum class Mode {
	RUNNING,
	PAUSED,
	MENU
};

class View {
public:
	virtual void init() = 0;
	virtual void drawAt(V pos, V size, const MapObject* glyph) = 0;
	virtual void pause() = 0;
};