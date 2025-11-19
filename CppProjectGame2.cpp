#include "GameView.h"

void start_menu(GameView* game); // menu.cpp
void init_level1(GameView* game); // level1.cpp

int main()
{
	GameView game;
	init_level1(&game);
	start_menu(&game);
	return 0;
}