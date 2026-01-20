#include "GameView.h"
#include "GameParser.h"
#include "Console.h"
#include "GameRunner.h"


// Application top-level main loop. init callback configures a new GameView.
void main_loop(const GameParser& parser, GameRunner*& runner) {
    if (!runner) runner = new KeyboardGameRunner();
    ConsoleMenu::Mode mode = ConsoleMenu::menu(runner);
    GameView* game = nullptr;
    std::string exit_msg = runner->get_exit_msg();
    try {
        while ((bool)mode) {
            switch (mode) {
            case ConsoleMenu::Mode::RUNNING:
                if (game) delete game;
                game = new GameView(runner);
                parser.init_game(game);
                mode = game->run();
                break;
            case ConsoleMenu::Mode::CONTINUE:
                if (game) mode = game->run();
                else mode = ConsoleMenu::Mode::RUNNING;
                break;
            case ConsoleMenu::Mode::PAUSED:
                mode = ConsoleMenu::pause(runner);
                break;
            case ConsoleMenu::Mode::MENU:
                if (game) delete game;
                game = nullptr;
                mode = ConsoleMenu::menu(runner);
                break;
            case ConsoleMenu::Mode::WINNING:
                ConsoleMenu::won_game();
                while (ConsoleMenu::get_keypress() == ConsoleMenu::Keypress::NONE);
                mode = ConsoleMenu::Mode::MENU;
                break;
            default:
                mode = ConsoleMenu::Mode::EXIT;
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

// Main function: the entry point of the application. Parses command-line arguments
// and starts the main loop of the game with the appropriate runner configuration.
int main(int argc, char* argv[]) {
    GameRunner* runner = nullptr;
    GameParser parser;

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "-load") {
            if (argc > 2 && std::string(argv[2]) == "-silent") runner = new TestGameRunner();
            else runner = new LoadedThenContinueGameRunner();
        }
        else if (arg == "-save") runner = new SavingGameRunner();
    }

    // Start the main loop of the application with the parser and runner instances.
    main_loop(parser, runner);

    delete runner;
    return 0;
}