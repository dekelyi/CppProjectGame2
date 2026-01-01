#include "GameView.h"
#include "LevelParser.h"
#include <functional>

void _main(std::function<void(GameView*)>); // Runner.cpp


int main()
{
	RiddleParser riddle_parser("riddles.txt");
	riddle_parser.parse();
	_main([&](GameView* game) {LevelParser::parse_all_levels(game, riddle_parser);});
	return 0;
}