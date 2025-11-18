#pragma once
#include "Vector.h"
#include "MapObj.h"
#include "View.h"

class ConsoleView : public View {
public:
	virtual void init() override;
	virtual void drawAt(V pos, V size, const MapObject* glyph) override;
	virtual void pause() override;
};