#pragma once
#include <set>
#include "Object.h"
#include "Msg.h"
#include "DrawingBuffer.h"
#include "PlayersProp.h"
#include "DoorProp.h"
#include "TorchProp.h"
#include <functional>

class GameView;

/** A single game room */
class GameRoom {
	friend class GameView;
	friend class Torch;
public:
	const unsigned X, Y, LEGEND_Y_POS;

	std::set<MapObject*> map_objects; // all objects currently in this room
	MapBuffer drawing_buffer;
	// linked list
	GameRoom* next = nullptr;
	GameRoom* prev = nullptr;

	DoorProp p_doors = { *this };
	PlayersProp p_players = { *this };
	TorchProp p_torch = { *this };

	bool is_current = false;
	Msg msg;

	inline GameRoom(unsigned x, unsigned y, unsigned legend_pos) : X(x), Y(y), LEGEND_Y_POS(legend_pos), drawing_buffer(MapBuffer(x, y, legend_pos)) {}

	inline void init(unsigned int i) {
		p_doors.init(i);
	}

	inline void add_object(MapObject* obj) {
		map_objects.insert(obj);
		p_doors.add_object(obj);
	}

	inline void remove_object(MapObject* obj, bool del = true) {
		map_objects.erase(obj);
		if (!p_players.remove_object(obj) && del) delete obj;
	}

	template <typename T = MapObject> inline std::set<T*> get_objects() const {
		std::set<T*> objs;
		std::set<MapObject*> map_objects = this->map_objects,
			collectibles = p_players.get_objects();
		map_objects.insert(collectibles.begin(), collectibles.end());
		
		for (MapObject* obj : map_objects) {
			T* casted = dynamic_cast<T*>(obj);
			if (casted != nullptr)
				objs.insert(casted);
		}
		return objs;
	}
	inline void drawBuffer(std::function<void(unsigned)> draw_hud) const {
		p_torch.manipulate_buffer().draw(draw_hud);
	}
	inline void draw(const MapObject& obj) {
		drawing_buffer.set_at(obj.getPosition(), obj.getSize(), { obj.getGlyph(), obj.getAttr() });
	}
	inline void clear(const MapObject& obj, const std::string attr = "") {
		drawing_buffer.set_at(obj.getPosition(), obj.getSize(), DNULL);
	}

	inline MapObject* get_object_at(V pos) const {
		for (MapObject* obj : map_objects)
			if (obj->is_at(pos))
				return obj;
		return nullptr;
	}
};