#pragma once
#include <vector>
#include "Object.h"
#include "player.h"
#include "Collectible.h"
#include "DoorProp.h"

class GameView;

/** A single game room */
class GameRoom : public DoorProp {
	friend class GameView;
	friend class Torch;
	std::vector<MapObject*> map_objects; // all objects currently in this room

public:
	// linked lsit
	GameRoom* next = nullptr;
	GameRoom* prev = nullptr;

	Player* player1 = nullptr;
	Player* player2 = nullptr;

	bool is_dark = false;
	bool is_current = false;
	std::string msg = "";

	GameRoom(Player* _player1, Player* _player2) : player1(_player1), player2(_player2) {}
	inline ~GameRoom() {
		for (auto o : map_objects) if (o != player1 && o != player2) delete o;
	}

	inline void init(unsigned int i) {
		DoorProp::init(i);
	}

	inline void add_object(MapObject* obj) {
		map_objects.push_back(obj);
		DoorProp::add_object(obj);
	}


	inline void remove_object(MapObject* obj) {
		map_objects.erase(std::remove(map_objects.begin(), map_objects.end(), obj));
	}

	// Drawing stuff
	inline void draw(const MapObject& obj, VS* dims = nullptr) const {
		if (dims == nullptr) dims = new VS(should_draw_object(&obj));
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

	std::vector<Torch*> get_torches() const {
		std::vector<Torch*> torches;
		for (MapObject* obj : map_objects) {
			Torch* t = dynamic_cast<Torch*>(obj);
			if (t != nullptr)
				torches.push_back(t);
		}
		for (Player* p : { player1, player2 }) {
			Torch* t = dynamic_cast<Torch*>(p->collectible);
			if (t != nullptr) {
				t->setPosition(p->getPosition());
				torches.push_back(t);
			}
		}
		return torches;
	}

	inline VS should_draw_object(const MapObject* val) const {
		VS r = { V(0,0), V(0,0) };
		std::vector<Torch*> torches = get_torches();
		for (MapObject* obj : map_objects)
			if (obj == val) {
				if (!is_dark) r = { obj->getPosition(), obj->getSize() };
				else {
					for (Torch* t : torches) {
						VS vs = is_in_bounds_dim(val->getPosition(), t->getPosition(), t->getSize(), t->area);
						if (vs.size.getX() != 0 && vs.size.getY() != 0) {
							r = vs;
							break;
						}
					}
				}
				break;
			}
		return r;
	}

	inline int count_players() {
		int count_players = 0;
		for (MapObject* obj : map_objects) {
			Player* p = dynamic_cast<Player*>(obj);
			if (p != nullptr) count_players++;
		}
		return count_players;
	}
};