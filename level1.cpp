#include "Game.h"
#include "Object.h"
#include "player.h"
#include "Obstacle.h"


void init_level1(Game* game) {
	game->player1->setPosition(V(10, 10));
	game->addObject(new MapObject(V(20, 20), V(10, 1), (char)ObjType::WALL));
	game->addObject(new Obstacle(V(5, 5), V(1, 1)));
}