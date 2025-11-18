#pragma once
#include "MapObj.h"
#include "Screen.h"
#include <vector>

class Map {
	std::vector<MapObject> map_objects;

public:
	Map() {};

	inline void addObject(MapObject obj) {
		map_objects.push_back(obj);
		drawObj(&obj);
	}

	inline void draw(V pos, V size, MapObject* obj) { drawAt(pos, size, (obj ? obj->getGlyph() : ' ')); }
	inline void drawObj(MapObject* obj) { draw(obj->getPosition(), obj->getSize(), obj); }
	inline void clearObj(MapObject* obj) { draw(obj->getPosition(), obj->getSize(), nullptr); }
	
	MapObject* get_object_at(V pos) {
		for (auto& obj : map_objects)
			if (obj.is_at(pos))
				return &obj;
		return nullptr;
	}

	void init();
	void loop();
};