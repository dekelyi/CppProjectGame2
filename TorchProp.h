#pragma once
#include "DrawingBuffer.h"
#include "BaseProp.h"
#include "Collectible.h"

static const DrawingObject SHADOW = DrawingObject(' ', A_BACKGROUND_GREY);

class TorchProp : public BaseProp {
public:
	bool is_dark = false;

	MapBuffer<SIZE_X, SIZE_Y> manipulate_buffer() const;
};