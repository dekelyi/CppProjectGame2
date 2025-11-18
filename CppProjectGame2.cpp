#include "Game.h"

void start_menu(Game* game); // runner.cpp
void init_level1(Game* game); // level1.cpp

int main()
{
	Game game;
	init_level1(&game);
	start_menu(&game);
	return 0;
}