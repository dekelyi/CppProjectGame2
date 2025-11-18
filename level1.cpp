#include "Map.h"
#include "MapObj.h"
#include "player.h"
#include "Obstacle.h"


void init_level1(Map* map) {
	player1.setPosition(V(10, 10));
	map->addObject(new MapObject(V(20, 20), V(10, 1), (char)ObjType::WALL));
	map->addObject(new Obstacle(V(5, 5), V(1, 1)));
	map->addObject(&player1);
}