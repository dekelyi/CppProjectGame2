#include "GameView.h"
#include "Object.h"
#include "Obstacle.h"
#include "Collectible.h"
#include "Door.h"

void init_level1(GameView* game) {
	game->player1->setPosition(V(10, 10));
	game->player2->setPosition(V(15, 10));
	game->current_room()->addObject(game->player1);
	game->current_room()->addObject(game->player2);
	game->current_room()->addObject(new MapObject(V(20, 19), V(10, 1), (char)ObjType::WALL));
	game->current_room()->addObject(new Obstacle(V(5, 5), V(1, 1)));
	game->current_room()->addObject(new Key(V(20,18), V(1, 1)));
	(new Door(V(70, 19), V(1, 1), DoorDest::NEXT))->add_to_room(game->current_room());
}

void init_level2(GameView* game) {
	GameRoom* room = game->add_room();
	room->addObject(new Obstacle(V(30, 10), V(1, 1)));
	room->addObject(new Key(V(40,15), V(1, 1)));
	(new Door(V(0, 0), V(1, 1), DoorDest::PREV))->add_to_room(room);
}

void init_levels(GameView* game) {
	init_level1(game);
	init_level2(game);
}