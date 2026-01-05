#pragma once
#include <vector>
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

	std::vector<MapObject*> map_objects; // all objects currently in this room
	MapBuffer drawing_buffer;
	// linked list
	GameRoom* next = nullptr;
	GameRoom* prev = nullptr;

	// Props (compostion)
	DoorProp p_doors = { *this };
	PlayersProp p_players = { *this };
	TorchProp p_torch = { *this };

	bool is_current = false;
	Msg* msg = nullptr;

	inline GameRoom(unsigned x, unsigned y, unsigned legend_pos) : X(x), Y(y), LEGEND_Y_POS(legend_pos), drawing_buffer(MapBuffer(x, y, legend_pos)), msg(new Msg()) {}

	~GameRoom() {
		delete msg;
	}

	// init the room properties
	inline void init(unsigned int i) {
		p_doors.init(i);
	}

	/****************** OBJECTS ****/

	// add an object to the room
	inline void add_object(MapObject* obj) {
		map_objects.push_back(obj);
		p_doors.add_object(obj);
	}

	// remvoe the object from the room.
	// if `del` is true, the object is deleted from memory 
	inline void remove_object(MapObject* obj, bool del = true) {
		//map_objects.erase(obj);
		map_objects.erase(std::remove(map_objects.begin(), map_objects.end(), obj));
		if (del && !p_players.remove_object(obj)) delete obj;
	}

	/**
	* Get all objects in the room, including players' collectibles of type `T`
	*/
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

	/**
	* Get the object at `pos`
	*/
	inline MapObject* get_object_at(V pos) const {
		for (MapObject* obj : map_objects)
			if (obj->is_at(pos))
				return obj;
		return nullptr;
	}

	/********* DRAWING ********/

	/**
	* manipulates the drawing buffer and drawing it
	*/
	inline void drawBuffer(std::function<void(unsigned)> draw_hud) const {
		p_torch.manipulate_buffer().draw(draw_hud);
	}

	/**
	* Draws to the drawing buffer
	*/
	inline void draw(const MapObject& obj) {
		drawing_buffer.set_at(obj.getPosition(), obj.getSize(), { obj.getGlyph(), obj.getAttr() });
	}

	/**
	* Clears from the drawing buffer
	*/
	inline void clear(const MapObject& obj, const std::string attr = "") {
		drawing_buffer.set_at(obj.getPosition(), obj.getSize(), DNULL);
	}
};