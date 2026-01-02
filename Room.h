#pragma once
#include <vector>
#include "Object.h"
#include "Msg.h"
#include "DrawingBuffer.h"
#include "PlayersProp.h"
#include "DoorProp.h"
#include "TorchProp.h"

class GameView;

/** A single game room */
class GameRoom {
	friend class GameView;
	friend class Torch;
public:
	const unsigned X, Y;

	std::vector<MapObject*> map_objects; // all objects currently in this room
	MapBuffer drawing_buffer;
	// linked list
	GameRoom* next = nullptr;
	GameRoom* prev = nullptr;

	DoorProp p_doors = { *this };
	PlayersProp p_players = { *this };
	TorchProp p_torch = { *this };

	bool is_current = false;
	Msg msg;

	inline GameRoom(unsigned x, unsigned y) : X(x), Y(y), drawing_buffer(MapBuffer(x, y)) {}

	inline void init(unsigned int i) {
		p_doors.init(i);
	}

	inline void add_object(MapObject* obj) {
		map_objects.push_back(obj);
		p_doors.add_object(obj);
	}


	inline bool remove_object(MapObject* obj) {
		map_objects.erase(std::remove(map_objects.begin(), map_objects.end(), obj));
		return p_players.handle_remove_obj(obj);
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
	inline void drawBuffer() const {
		p_torch.manipulate_buffer().draw();
	}
	inline void draw(const MapObject& obj) {
		drawing_buffer.set_at(obj.getPosition(), obj.getSize(), { obj.getGlyph(), obj.getAttr() });
	}
	inline void clear(const MapObject& obj, const std::string attr = "") {
		drawing_buffer.set_at(obj.getPosition(), obj.getSize(), DNULL);
	}

	inline MapObject* get_object_at(V pos) {
		for (MapObject* obj : map_objects)
			if (obj->is_at(pos))
				return obj;
		return nullptr;
	}
};