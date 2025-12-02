#include "GameView.h"

void _main(GameView* game); // menu.cpp
void init_levels(GameView* game); // level1.cpp

int main()
{
	GameView game;
	init_levels(&game);
	_main(&game);
	return 0;
}