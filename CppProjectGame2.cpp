#include "GameView.h"
#include "LevelParser.h"
#include "Console.h"
#include "GameRunner.h"

int main(int argc, char* argv[]) {
	RiddleParser riddle_parser("riddles.txt");
	riddle_parser.parse();
	GameRunner* runner = nullptr;
	if (argc > 1) {
		std::string arg = argv[1];
		if (arg == "-load") {
			if (argc > 2 && std::string(argv[2]) == "-silent") runner = new TestGameRunner();
			else runner = new LoadedGameRunner();
		}
		else if (arg == "-save") runner = new SavingGameRunner();
	}
	ConsoleMenu::main_loop([&](GameView* game) {LevelParser::parse_all_levels(game, riddle_parser);}, runner);
	return 0;
}