#include "GameView.h"
#include "Object.h"
#include "Obstacle.h"
#include "Collectible.h"
#include "Door.h"
#include "Spring.h"

void init_level1(GameView* game) {
	GameRoom* room = game->add_room();
	room->addObject(new MapObject(V(20, 19), S(10, S::SH), (char)ObjType::WALL));
	room->addObject(new MapObject(V(57, 8), S(10, S::SV), (char)ObjType::WALL));
	room->addObject(new Obstacle(V(5, 5), S1));
	room->addObject(new Key(V(20,18), S1));
	room->addObject(new Obstacle(V(20, 10), S1));
	room->addObject(new Door(V(70, 18), S(2, S::SV), DoorDest::NEXT));
	room->addObject(new Spring(V(50, 10), S(5, S::SH)));
}

void init_level2(GameView* game) {
	GameRoom* room = game->add_room();
	room->addObject(new Obstacle(V(30, 10), S1));
	room->addObject(new Key(V(40,15), S1));
	room->addObject(new Door(V(0, 0), S1, DoorDest::PREV));
	room->addObject(new Door(V(70, 19), S1, DoorDest::NEXT, 2));
}

void init_levels(GameView* game) {
	init_level1(game);
	init_level2(game);

	game->player1->setPosition(V(40, 10));
	game->player2->setPosition(V(42, 10));
	game->current->addObject(game->player1);
	game->current->addObject(game->player2);

}