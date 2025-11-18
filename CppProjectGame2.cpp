#include "Game.h"
#include "Console.h"

void init_level1(Game* game); // level1.cpp

int main()
{
	Game game;
	init_level1(&game);
	game.main();
	return 0;
}