#pragma once
#include <vector>
#include "Object.h"
#include "Msg.h"
#include "GameRunner.h"
#include "DrawingBuffer.h"
#include "PlayersProp.h"
#include "DoorProp.h"
#include "TorchProp.h"
#include <functional>

class GameView;

/**
 * GameRoom - single logical room/scene in the game.
 *
 * Responsibilities:
 * - Owns map objects and active props (Doors, Players, Torch) by composition.
 * - Provides helpers to add/remove objects and to draw into the room buffer.
 * - Coordinates room initialization and drawing hooks.
 *
 * Public API is minimal and lightweight; Props receive objects as needed.
 */
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

	// Props (composition)
	DoorProp p_doors = { *this };
	PlayersProp p_players = { *this };
	TorchProp p_torch = { *this };

	bool is_current = false;
	Msg* msg = nullptr;
	GameRunner* runner;

	inline GameRoom(unsigned x, unsigned y, unsigned legend_pos, GameRunner* _runner) : X(x), Y(y), LEGEND_Y_POS(legend_pos), drawing_buffer(MapBuffer(x, y, legend_pos)), msg(new Msg()), runner(_runner) {}

	~GameRoom() {
		delete msg;
	}

	/** Initialize room props (called by GameView::init_rooms). */
	inline void init(unsigned int i) {
		p_doors.init(i);
	}

	/****************** OBJECTS ****/

	// add an object to the room
	inline void add_object(MapObject* obj) {
		map_objects.push_back(obj);
		p_doors.add_object(obj);
	}

	/**
	 * Remove the object from the room.
	 * If `del` is true the object is deleted unless it was transferred to PlayersProp.
	 */
	inline void remove_object(MapObject* obj, bool del = true) {
		//map_objects.erase(obj);
		map_objects.erase(std::remove(map_objects.begin(), map_objects.end(), obj));
		if (del && !p_players.remove_object(obj)) delete obj;
	}

	/**
	 * Get all objects in the room, including players' collectibles of type `T`.
	 * Returns a vector of pointers to objects of type T.
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

	/** Return the object at the given position or nullptr. */
	inline MapObject* get_object_at(V pos) const {
		for (MapObject* obj : map_objects)
			if (obj->is_at(pos))
				return obj;
		return nullptr;
	}

	inline bool is_out_of_bounds(const V& v) const {
		return (unsigned)v.getX() >= X || (unsigned)v.getY() >= Y;
	}

	/********* DRAWING ********/

	/**
	 * Manipulate and draw the room's drawing buffer. `draw_hud` is a callback
	 * used to draw HUD overlay lines.
	 */
	inline void drawBuffer(std::function<void(unsigned)> draw_hud) const {
		p_torch.manipulate_buffer().draw(draw_hud);
	}

	/** Write a MapObject into the drawing buffer. */
	inline void draw(const MapObject& obj) {
		drawing_buffer.set_at(obj.getPosition(), obj.getSize(), { obj.getGlyph(), obj.getAttr() });
	}

	/** Clear object area from the drawing buffer. */
	inline void clear(const MapObject& obj, const std::string attr = "") {
		drawing_buffer.set_at(obj.getPosition(), obj.getSize(), DNULL);
	}
};