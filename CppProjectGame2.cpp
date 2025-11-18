#include "Map.h"
#include "Console.h"

void init_level1(Map* map); // level1.cpp

int main()
{
	Map map;
	init_level1(&map);
	map.main();
	return 0;
}