#pragma once
#include "BaseProp.h"
#include "Collectible.h"

class TorchProp : public BaseProp {
public:
	bool is_dark = false;

	VS get_drawing_dimensions(const VS& dimensions) const;
};