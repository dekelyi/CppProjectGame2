#include "GameView.h"
#include "Object.h"
#include "Obstacle.h"


void init_level1(GameView* game) {
	game->player1->setPosition(V(10, 10));
	game->player2->setPosition(V(15, 10));
	game->current_room()->addObject(new MapObject(V(20, 20), V(10, 1), (char)ObjType::WALL));
	game->current_room()->addObject(new Obstacle(V(5, 5), V(1, 1)));
}