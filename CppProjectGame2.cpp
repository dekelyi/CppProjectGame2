#include "GameView.h"

void _main(void (*fn)(GameView*)); // menu.cpp
void init_levels(GameView* game); // level1.cpp

int main()
{
	_main(init_levels);
	return 0;
}