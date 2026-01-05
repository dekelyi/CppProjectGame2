#include "GameView.h"
#include "LevelParser.h"
#include "Console.h"

int main()
{
	RiddleParser riddle_parser("riddles.txt");
	riddle_parser.parse();
	ConsoleMenu::main_loop([&](GameView* game) {LevelParser::parse_all_levels(game, riddle_parser);});
	return 0;
}