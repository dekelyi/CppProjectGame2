#include "TorchProp.h"
#include "Collectible.h"
#include "Room.h"

VS TorchProp::get_drawing_dimensions(const VS& dimensions) const {
	VS r = { V(0,0), V(0,0) };

	if (!is_dark) r = dimensions;
	else {
	std::vector<Torch*> torches = room.get_objects<Torch>();
		for (Torch* t : torches) {
			VS vs = is_in_bounds_dim(dimensions.pos, t->getPosition(), dimensions.size, t->area);
			if (vs.size.getX() != 0 && vs.size.getY() != 0) {
				r = vs;
				break;
			}
		}
	}

	return r;
	}