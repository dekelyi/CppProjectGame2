#pragma once
#define NOMINMAX // prevent windows.h from defining min/max macros
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <format>
#include <functional>
#include "Vector.h"
#include "Console.h"
#include "prelude.h"
#include "GameView.h"
#include "GameRunner.h"

using std::cout, std::endl, std::string, std::format;

// --------- Console (static helpers) --------------------

// Move the cursor to the given console coordinate and flush the output.
void Console::gotoxy(V pos) {
    cout.flush();
    COORD coord;
    coord.X = pos.getX();
    coord.Y = pos.getY();
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Show or hide the console cursor.
void Console::showCursor(bool show) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = show; // Set to TRUE to make it visible
    SetConsoleCursorInfo(hStdOut, &curInfo);
}

// Clear the console using system call.
void Console::cls() {
    system("cls");
}

// Sleep wrapper around Win32 Sleep.
void Console::sleep(int ms) {
    Sleep(ms);
}

// Initialize console state for the game: clear, hide cursor, enable ANSI codes.
void Console::init() {
    Console::cls();
    Console::showCursor(false);

    // Enable ANSI escape codes
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hStdOut, &dwMode);
    dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hStdOut, dwMode);
}

// Restore console state after the game.
void Console::deinit() {
    Console::cls();
    Console::showCursor(true);
}

// --------- Writer -----------------
// Write a line at the writer's current position and advance Y.
void Writer::writeline(const string& line) {
    Console::gotoxy(pos);
    cout << line;
    pos.setY(pos.getY() + 1);
}

// --------- ConsoleMenu (static class) -----------------

bool ConsoleMenu::colors = true;

// Pause dialog: blocking loop that returns selected next mode.
Mode ConsoleMenu::pause() {
    Console::cls();
    Console::gotoxy(V(10, 5));
    cout << "Game paused, press ESC again to continue or H to go back to the main menu" << endl;

    Mode m = Mode::PAUSED;
    while (m == Mode::PAUSED) {
        Keypress e = ConsoleMenu::get_keypress();
        if (e == Keypress::ESC) m = Mode::CONTINUE;
        if (e == Keypress::H) m = Mode::MENU;
    }
    return m;
}

// Show win screen (non-blocking).
void ConsoleMenu::won_game() {
    Console::init();
    Writer w = { V(10, 5) };
    w.writeline("You won the Game");
    w.writeline("press any key to return to the main menu");
}

// Display manual instructions (blocking).
void ConsoleMenu::manual() {
    Console::init();
    Writer w = { V(10, 5) };
    w.writeline("Instructions:");
    w.writeline("Player 1 controls: W (up), A (left), S (stay), D (right), X (down), E (dispose)");
    w.writeline("Player 2 controls: I (up), J (left), K (stay), L (right), M (down), O (dispose)");
    w.writeline("Press any key to return to the main menu...");
    _getch();
}

// Return Keypress enum from keyboard; returns NONE if no key available.
Keypress ConsoleMenu::get_keypress() {
    if (_kbhit()) {
        char ch = _getch();
        return (Keypress)(toupper(ch));
    }
    return Keypress::NONE;
}

// Main menu flow; returns chosen Mode.
Mode ConsoleMenu::menu() {
    Console::init();
    Writer w = { V(10, 5) };
    w.writeline("Welcome to the Game!");
    w.writeline("(1) Start a new game");
    w.writeline(format("(7) Colors: turn {}", colors ? "off" : "on"));
    w.writeline("(8) Instruction and manual");
    w.writeline("(9) Exit");

    Mode m = Mode::MENU;
    while (m == Mode::MENU) {
        Keypress e = ConsoleMenu::get_keypress();
        if (e == Keypress::_1) m = Mode::RUNNING;
        if (e == Keypress::_7) { // disable/enable colors
            ConsoleMenu::colors = !ConsoleMenu::colors;
            ConsoleMenu::menu();
        }
        if (e == Keypress::_8) { // print manual
            ConsoleMenu::manual();
            ConsoleMenu::menu();
        }
        if (e == Keypress::_9 || e == Keypress::ESC) m = Mode::EXIT;
    }
    return m;
}



// Application top-level main loop. init callback configures a new GameView.
void ConsoleMenu::main_loop(std::function<void(GameView*)> init, GameRunner* runner) {
    Mode mode = runner->get_mode(Mode::MENU);
    GameView* game = nullptr;
    std::string exit_msg = runner->get_exit_msg();
    try {
        while ((bool)mode) {
            switch (mode) {
            case Mode::RUNNING:
                if (game) delete game;
                game = new GameView(runner);
                try {
                    init(game);
                }
                catch (const std::runtime_error& e) {
                    Console::deinit();
                    std::cout << e.what();
                    return;
                }
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
	} catch (const EventAssertionError& e) {
		exit_msg = format("Event assertion error: {}\n", e.what());
    }
    catch (const std::runtime_error& e) {
        exit_msg = format("Runtime error: {}\n", e.what());
    } catch (const std::exception& e) {
        exit_msg = format("Fatal error: {}\n", e.what());
	}
    if (game) delete game;
    Console::deinit();
	if (!exit_msg.empty()) {
        std::cout << exit_msg;
    }
}