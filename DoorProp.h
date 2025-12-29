#pragma once
#include "BaseProp.h"
#include "Door.h"

class DoorProp : public BaseProp {
public:
	Door* entry_point = nullptr;
	Door* exit_point = nullptr;
	Door* last_moved_through = nullptr;

	inline void add_object(MapObject* obj) {
		Door* door = dynamic_cast<Door*>(obj);
		if (door) {
			if (door->dest == DoorDest::NEXT)
				this->exit_point = door;
			else
				this->entry_point = door;
		}
	}

	inline void init(unsigned int i) {
		// set current doors glyphs
		if (entry_point) entry_point->setGlyph('0' + i - 1);
		if (exit_point) exit_point->setGlyph('0' + i + 1);
	}
};