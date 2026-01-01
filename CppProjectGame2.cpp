#include "GameView.h"
#include "LevelParser.h"
#include <functional>

void _main(std::function<void(GameView*)>); // Runner.cpp
void init_levels(GameView* game); // level1.cpp

int main()
{
	RiddleParser riddle_parser("riddles.txt");
	riddle_parser.parse();
	LevelParser parser("adv-world_01.screen", riddle_parser);
	parser.parse();
	int x = 0;
	_main([&](GameView* game) {parser.build_room(game);});
	return 0;
}