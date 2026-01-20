#pragma once
#include "DrawingBuffer.h"
#include "BaseProp.h"

// Shadow drawing object used to darken non-visible cells
static const DrawingObject SHADOW = DrawingObject(' ', A_BACKGROUND_GREY);

class TorchProp : public BaseProp {
public:
    bool is_dark = false; // whether the room is currently dark

    /**
     * Returns a manipulated copy of the room's drawing buffer applying
     * darkness and torch light effects. The returned MapBuffer is a copy and
     * can be safely modified by the caller.
     */
    MapBuffer manipulate_buffer() const;
};