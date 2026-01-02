#include "TorchProp.h"
#include "Collectible.h"
#include "Room.h"

MapBuffer TorchProp::manipulate_buffer() const
{
	MapBuffer buffer = room.drawing_buffer;
	if (!is_dark) return buffer;

	for (unsigned x = 0; x < buffer.X; x++)
		for (unsigned y = 0; y < buffer.Y; y++)
			if (buffer.is_set(V(x, y))) buffer.set_at(V(x, y), SHADOW);

	for (auto& torch : room.get_objects<Torch>()) {
		unsigned area = torch->area;
		V start_pos = torch->getPosition() - V(area, area);
		for (unsigned dy = 0; dy < area*2; dy++)
			for (unsigned dx = 0; dx < area * 2; dx++) {
				V pos = start_pos + V(dx, dy);
				buffer.set_at(pos, room.drawing_buffer.get_at(pos));
			}
	}
	return buffer;
}
