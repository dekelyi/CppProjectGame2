#include "GameView.h"
#include "GameParser.h"
#include "Console.h"
#include "GameRunner.h"


// Application top-level main loop. init callback configures a new GameView.
void main_loop(const GameParser& parser, GameRunner* runner) {
    Mode mode = runner->get_mode(Mode::MENU);
    GameView* game = nullptr;
    std::string exit_msg = runner->get_exit_msg();
    try {
        while ((bool)mode) {
            switch (mode) {
            case Mode::RUNNING:
                if (game) delete game;
                game = new GameView(runner);
                parser.init_game(game);
                mode = game->run();
                break;
            case Mode::CONTINUE:
                if (game) mode = game->run();
                else mode = Mode::RUNNING;
                break;
            case Mode::PAUSED:
                mode = ConsoleMenu::pause();
                break;
            case Mode::MENU:
                if (game) delete game;
                game = nullptr;
                mode = ConsoleMenu::menu();
                break;
            case Mode::WINNING:
                ConsoleMenu::won_game();
                while (ConsoleMenu::get_keypress() == Keypress::NONE);
                mode = Mode::MENU;
                break;
            default:
                mode = Mode::EXIT;
                break;
            }
        }
    }
    catch (const EventAssertionError& e) {
        exit_msg = format("Event assertion error: {}\n", e.what());
    }
    catch (const std::runtime_error& e) {
        exit_msg = format("Runtime error: {}\n", e.what());
    }
    catch (const std::exception& e) {
        exit_msg = format("Fatal error: {}\n", e.what());
    }
    if (game) delete game;
    Console::deinit();
    if (!exit_msg.empty()) {
        std::cout << exit_msg;
    }
}

int main(int argc, char* argv[]) {
	GameRunner* runner = nullptr;
	GameParser parser;

	if (argc > 1) {
		std::string arg = argv[1];
		if (arg == "-load") {
			if (argc > 2 && std::string(argv[2]) == "-silent") runner = new TestGameRunner();
			else runner = new LoadedGameRunner();
		}
		else if (arg == "-save") runner = new SavingGameRunner();
	}

	main_loop(parser, runner);

	delete runner;
	return 0;
}