#include "prelude.h"
#include "Collectible.h"
#include "Door.h"
#include "Obstacle.h"
#include "Spring.h"
#include "Switch.h"
#include "Riddle.h"
#include "LevelParser.h"

MapObject* ObjectData::into_map_object(GameView* game, GameRoom* room, const LevelParser& parser) const {
	switch (type) {
	case ObjType::PLAYER_1:
	case ObjType::PLAYER_2: {
		auto p = (type == ObjType::PLAYER_1) ? game->player1 : game->player2;
		p->setPosition(position);
		room->add_object(p);
		return p;
	}
	case ObjType::DOOR: {
		int required_keys;
		try {
			required_keys = stoi(properties.at("condition-keys"));
		}
		catch (...) {
			required_keys = 0;
		}
		DoorDest DEST = (id == -1) ? DoorDest::NEXT : DoorDest::PREV;
		auto door = new Door(position, size, DEST, required_keys);
		return door;
	}
	case ObjType::OBSTCALE:
		return new Obstacle(position, size);
	case ObjType::SPRING:
		return new Spring(position, size);
	case ObjType::SWITCH_OFF:
		if (properties.count("do-open-door")) {
			string door_id = properties.at("do-open-door");
			auto cond = new Condition(Condition::SWITCH, 1);
			Door* door = (door_id == "N") ? room->p_doors.exit_point : room->p_doors.entry_point;
			door->conditions.push_back(cond);
			auto cb = SwitchDoor(room, door, cond);
			return new Switch(position, cb);
		}
		return new Switch(position);
	case ObjType::TORCH:
		return new Torch(position, stoi(properties.at("area")));
	case  ObjType::KEY:
		return new Key(position, size);
	case ObjType::RIDDLE: {
		RiddleData rdata = parser.riddle_parser.riddles.at(id);
		return new Riddle(position, size, rdata);
	}
	default:
		return new MapObject(position, size, (char)type);
	}
}