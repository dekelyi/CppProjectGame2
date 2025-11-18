#pragma once
#include "MapObj.h"
#include "Screen.h"
#include <vector>

class Map {
	std::vector<MapObject*> map_objects;

public:
	Map() {};

	inline void addObject(MapObject* obj) {
		map_objects.push_back(obj);
		drawObj(obj);
	}

	inline void draw(V pos, V size, const MapObject* obj) { drawAt(pos, size, (obj ? obj->getGlyph() : ' ')); }
	inline void drawObj(const MapObject* obj) { draw(obj->getPosition(), obj->getSize(), obj); }
	inline void clearObj(const MapObject* obj) { draw(obj->getPosition(), obj->getSize(), nullptr); }
	
	MapObject* get_object_at(V pos) {
		for (MapObject* obj : map_objects)
			if (obj->is_at(pos))
				return obj;
		return nullptr;
	}

	void init();
	void loop();
};