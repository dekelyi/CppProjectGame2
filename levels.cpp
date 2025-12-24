#include "GameView.h"
#include "Object.h"
#include "Obstacle.h"
#include "Collectible.h"
#include "Door.h"
#include "Spring.h"
#include "Switch.h"

void init_level1(GameView* game) {
	GameRoom* room = game->add_room();
	room->is_dark = true;
	room->add_object(new MapObject(V(20, 19), S(10, S::SH), (char)ObjType::WALL));
	room->add_object(new MapObject(V(57, 8), S(10, S::SV), (char)ObjType::WALL));
	room->add_object(new Obstacle(V(5, 5), S1));
	room->add_object(new Key(V(20,18), S1));
	room->add_object(new Obstacle(V(20, 10), S1));
	room->add_object(new Spring(V(30, 19), S(5, S::SH)));
	room->add_object(new Torch(V(38, 10), 3));

	auto condition_switch = new Condition(Condition::SWITCH, 1);
	auto door = new Door(V(70, 18), S(2, S::SV), DoorDest::NEXT);
	room->add_object(new Switch(V(50, 10), SwitchDoor(room, door, condition_switch)));
	door->conditions.push_back(condition_switch);
	room->add_object(door);
}

void init_level2(GameView* game) {
	GameRoom* room = game->add_room();
	room->add_object(new Obstacle(V(30, 10), S1));
	room->add_object(new Key(V(40,15), S1));
	room->add_object(new Door(V(0, 0), S1, DoorDest::PREV));
	room->add_object(new Door(V(70, 19), S1, DoorDest::NEXT, 2));
}

void init_level3(GameView* game) {
	GameRoom* room = game->add_room();
	room->add_object(new Door(V(70, 19), S1, DoorDest::NEXT));
}

void init_levels(GameView* game) {
	init_level1(game);
	init_level2(game);
	init_level3(game);

	game->player1->setPosition(V(40, 10));
	game->player2->setPosition(V(42, 10));
	game->current->add_object(game->player1);
	game->current->add_object(game->player2);

}