#pragma once
#define NOMINMAX // prevent windows.h from defining min/max macros
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <format>
#include <filesystem>
#include <vector>

#include "Console.h"
#include "Vector.h"
#include "GameRunner.h"

using std::cout, std::endl, std::string, std::format;
namespace fs = std::filesystem;

// --------- Console (static helpers) --------------------

/**
 * Move the cursor to the given console coordinate and flush the output.
 */
void Console::gotoxy(V pos) {
    cout.flush();
    COORD coord;
    coord.X = pos.getX();
    coord.Y = pos.getY();
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/**
 * Show or hide the console cursor.
 * @param show true to show cursor, false to hide.
 */
void Console::showCursor(bool show) {
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = show; // Set to TRUE to make it visible
    SetConsoleCursorInfo(hStdOut, &curInfo);
}

/**
 * Clear the console screen using system call.
 */
void Console::cls() {
    system("cls");
}

/**
 * Sleep wrapper around Win32 Sleep.
 * @param ms milliseconds to sleep
 */
void Console::sleep(int ms) {
    Sleep(ms);
}

/**
 * Initialize console state for the game: clear, hide cursor, enable ANSI codes.
 */
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

/**
 * Restore console state after the game.
 */
void Console::deinit() {
    Console::cls();
    Console::showCursor(true);
}

// --------- Writer -----------------
/**
 * Write a line at the writer's current position and advance Y coordinate.
 */
void Writer::writeline(const string& line) {
    Console::gotoxy(pos);
    cout << line;
    pos.setY(pos.getY() + 1);
}

// --------- ConsoleMenu (static class) -----------------

bool ConsoleMenu::colors = true;

/**
 * Pause dialog: blocking loop that returns selected next mode.
 * Offers saving via 'S' key which calls runner->save_steps().
 */
ConsoleMenu::Mode ConsoleMenu::pause(GameRunner* runner) {
    Mode m = Mode::PAUSED;
    while (m == Mode::PAUSED) {
        Console::init();
		Writer wr = { V(10, 5) };
		wr.writeline("Game paused: ");
		wr.writeline("(ESC) to continue");
		wr.writeline("(H) to go back to the main menu");
		wr.writeline("(S) to save the current game to file");

        Keypress e;
        while (!(bool)(e = ConsoleMenu::get_keypress()));
        if (e == Keypress::ESC) m = Mode::CONTINUE;
        if (e == Keypress::H) m = Mode::MENU;
        if (e == Keypress::STAY_1) { // 'S' pressed
            try {
                Console::showCursor(true);
                cout << "Enter filename to save (or press Enter for " << STEPS_FILENAME << "): ";
                std::string fname;
                std::getline(std::cin, fname);
                if (fname.empty()) fname = STEPS_FILENAME;
                runner->save_steps(fname);
                cout << "Saved steps to " << fname << "\n";
                Console::showCursor(false);
                Console::sleep(500);
            }
            catch (const std::exception& ex) {
                cout << "Failed to save steps: " << ex.what() << "\n";
            }
        }
    }
    return m;
}

/**
 * Show win screen (non-blocking).
 */
void ConsoleMenu::won_game() {
    Console::init();
    Writer w = { V(10, 5) };
    w.writeline("You won the Game");
    w.writeline("press any key to return to the main menu");
}

/**
 * Display manual instructions (blocking).
 */
void ConsoleMenu::manual() {
    Console::init();
    Writer w = { V(10, 5) };
    w.writeline("Instructions:");
    w.writeline("Player 1 controls: W (up), A (left), S (stay), D (right), X (down), E (dispose)");
    w.writeline("Player 2 controls: I (up), J (left), K (stay), L (right), M (down), O (dispose)");
    w.writeline("Press any key to return to the main menu...");
    _getch();
}

/**
 * Return Keypress enum from keyboard; returns NONE if no key available.
 */
ConsoleMenu::Keypress ConsoleMenu::get_keypress() {
    if (_kbhit()) {
        char ch = _getch();
        return (Keypress)(toupper(ch));
    }
    return Keypress::NONE;
}

/**
 * Main menu flow; returns chosen Mode.
 */
ConsoleMenu::Mode ConsoleMenu::menu(GameRunner*& runner) {
    Mode m = Mode::MENU;
    while (m == Mode::MENU) {
        Console::init();
        Writer w = { V(10, 5) };
        w.writeline("Welcome to the Game!");
        w.writeline("(1) Start a new game");
        w.writeline("(2) Load game from saved states");
        w.writeline(format("(7) Colors: turn {}", colors ? "off" : "on"));
        w.writeline("(8) Instruction and manual");
        w.writeline("(9) Exit");

        Keypress e;
		while (!(bool)(e = ConsoleMenu::get_keypress()));

        if (e == Keypress::_1) m = Mode::RUNNING;
        if (e == Keypress::_7) { // disable/enable colors
            ConsoleMenu::colors = !ConsoleMenu::colors;
        }
        if (e == Keypress::_8) { // print manual
            ConsoleMenu::manual();
        }
        if (e == Keypress::_9 || e == Keypress::ESC) m = Mode::EXIT;
        if (e == Keypress::_2) {
            // list .steps files in current directory
            std::vector<std::string> files;
            for (auto& p : fs::directory_iterator(fs::current_path())) {
                if (!p.is_regular_file()) continue;
                if (p.path().extension() == ".steps") files.push_back(p.path().string());
            }
            if (files.empty()) {
                Console::gotoxy(V(10, 12));
                cout << "No saved states found (no .steps files)." << endl;
                continue;
            }
            // display files numbered 1..n (up to 9)
            Console::cls();
            Console::gotoxy(V(10, 5));
            cout << "Select saved state to load (press number 1-9):" << endl;
            size_t maxShow = std::min(files.size(), (size_t)9);
            for (size_t i = 0; i < maxShow; ++i) {
                cout << "(" << (i+1) << ") " << files[i] << endl;
            }
            // wait for numeric selection
            while (true) {
                Keypress k = ConsoleMenu::get_keypress();
                if (k == Keypress::NONE) continue;
                if (k >= Keypress::_1 && k <= Keypress::_9) {
                    int idx = (int)k - (int)Keypress::_1;
                    if ((size_t)idx < files.size()) {
                        if (runner) delete runner;
                        runner = new HybridGameRunner(files[idx]);
                        m = Mode::RUNNING;
                        break;
                    }
                }
				if (k == Keypress::ESC) break; // cancel loading
            }
        }
    }
    return m;
}