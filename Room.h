#pragma once
#include <vector>
#include "Object.h"
#include "Msg.h"
#include "PlayersProp.h"
#include "DoorProp.h"
#include "TorchProp.h"

class GameView;

/** A single game room */
class GameRoom {
	friend class GameView;
	friend class Torch;
public:
	std::vector<MapObject*> map_objects; // all objects currently in this room
	// linked list
	GameRoom* next = nullptr;
	GameRoom* prev = nullptr;

	DoorProp p_doors = { *this };
	PlayersProp p_players = { *this };
	TorchProp p_torch = { *this };

	bool is_current = false;
	Msg* msg = new Msg();

	inline void init(unsigned int i) {
		p_doors.init(i);
	}

	inline void add_object(MapObject* obj) {
		map_objects.push_back(obj);
		p_doors.add_object(obj);
	}


	inline void remove_object(MapObject* obj) {
		map_objects.erase(std::remove(map_objects.begin(), map_objects.end(), obj));
	}

	template <typename T = MapObject> inline std::vector<T*> get_objects() const {
		std::vector<T*> objs;
		std::vector<MapObject*> map_objects = this->map_objects,
			collectibles = p_players.get_objects();
		map_objects.insert(map_objects.end(), collectibles.begin(), collectibles.end());
		
		for (MapObject* obj : map_objects) {
			T* casted = dynamic_cast<T*>(obj);
			if (casted != nullptr)
				objs.push_back(casted);
		}
		return objs;
	}

	// Drawing stuff
	inline VS get_drawing_dimensions(const MapObject* obj) const {
		VS r = { V(0,0), V(0,0) };
		if (std::find(map_objects.begin(), map_objects.end(), obj) == map_objects.end())
			return r;
		r = { obj->getPosition(), obj->getSize() };
		return p_torch.get_drawing_dimensions(r);

	}
	inline void draw(const MapObject& obj, VS* dims = nullptr) const {
		if (dims == nullptr) dims = new VS(get_drawing_dimensions(&obj));
		if (this->is_current && dims->size.getX() != 0 && dims->size.getY() != 0)
			ConsoleView::drawAt(dims->pos, dims->size, obj.getGlyph(), ConsoleView::colors ? obj.getAttr() : "");
	}
	inline void clear(const MapObject& obj, const std::string attr = "") const {
		if (this->is_current)
			ConsoleView::drawAt(obj.getPosition(), obj.getSize(), ' ', attr);
	}

	inline MapObject* get_object_at(V pos) {
		for (MapObject* obj : map_objects)
			if (obj->is_at(pos))
				return obj;
		return nullptr;
	}
};