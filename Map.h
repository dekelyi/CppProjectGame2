#pragma once
#include <vector>
#include "MapObj.h"
#include "View.h"

class Map {
	std::vector<MapObject*> map_objects;
	std::vector<View*> views;

public:
	Map() {};

	inline void add_view(View* view) {
		views.push_back(view);
	}

	inline void addObject(MapObject* obj) {
		map_objects.push_back(obj);
	}

	inline void draw(V pos, V size, const MapObject* obj) { 
		for (View* view : views)
			view->drawAt(pos, size, obj);
	}
	inline void drawObj(const MapObject* obj) { draw(obj->getPosition(), obj->getSize(), obj); }
	inline void clearObj(const MapObject* obj) { draw(obj->getPosition(), obj->getSize(), nullptr); }
	inline void drawAll() {
		for (MapObject* obj : map_objects)
			drawObj(obj);
	}
	
	MapObject* get_object_at(V pos) {
		for (MapObject* obj : map_objects)
			if (obj->is_at(pos))
				return obj;
		return nullptr;
	}

	void init();
	void main();
	Mode run();
};