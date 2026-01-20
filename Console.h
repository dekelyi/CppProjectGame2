/**
 * Console view helpers and drawers
 */
#pragma once
#include <string>
#include "Vector.h"

 /** --- CONSTANTS - ANSI CODES */
#define ANSI_ESC "\033["
#define A_FOREGROUND_RED ANSI_ESC "31m"
#define A_FOREGROUND_GREEN ANSI_ESC "32m"
#define A_FOREGROUND_YELLOW ANSI_ESC "33m"
#define A_FOREGROUND_BLUE ANSI_ESC "34m"
#define A_FOREGROUND_MAGENTA ANSI_ESC "35m"
#define A_FOREGROUND_CYAN ANSI_ESC "36m"
#define A_BACKGROUND_GREY ANSI_ESC "100m"
#define A_UNDERSCORE ANSI_ESC "4m"
#define A_BOLD ANSI_ESC "1m"
#define A_RESET ANSI_ESC "0m"

#define CH_BLOCK (char)178
#define CH_BLOCK_GREY (char)176

class GameView;
class GameRunner;
class GameParser;

/**
 * Console - static helper class that replaces previous free functions.
 *
 * Responsibilities:
 * - Terminal initialization / teardown used by the game loop.
 * - Cursor positioning and simple screen utilities.
 *
 * All methods are static convenience wrappers over Win32 console APIs.
 */
class Console {
public:
    /**
     * Initialize the console for the game (clear screen, hide cursor,
     * enable ANSI escape processing).
     */
    static void init();
    /**
     * Restore console state after the game (clear screen, show cursor).
     */
    static void deinit();

    /**
     * Move the terminal cursor to the given screen coordinates.
     * @param pos Target coordinate in console cells.
     */
    static void gotoxy(V pos);
    /**
     * Show or hide the terminal cursor.
     * @param show true to show the cursor, false to hide it.
     */
    static void showCursor(bool show);
    /**
     * Clear the terminal screen.
     */
    static void cls();
    /**
     * Sleep for the specified number of milliseconds.
     * @param ms Milliseconds to sleep.
     */
    static void sleep(int ms);
};

/**
 * A Writer that starts at a given position and retains the column during
 * multiple writes. Intended for simple vertical text output (writeline).
 */
class Writer {
    V pos;
public:
    Writer(V _pos) : pos(_pos) {}
    /**
     * Write a single line at the current writer position and advance the Y
     * coordinate by one.
     * @param line Text to print.
     */
    void writeline(const std::string& line);
};

/**
 * ConsoleMenu - static helper for the main menu and modal dialogs.
 *
 * Provides simple blocking menu flows used by the application bootstrap
 * and the main game loop. All methods are static and use the Console   
 * and Writer helpers for presentation.
 */
class ConsoleMenu {
public:
    /**
    * Represent the current runner mode
    */
    enum class Mode {
        EXIT = 0,
        RUNNING,
        PAUSED,
        CONTINUE,
        MENU,
        WINNING,
    };

    /**
    * Represent a keypress
    */
    enum class Keypress {
        // Player 1
        UP_1 = 'W',
        DOWN_1 = 'X',
        LEFT_1 = 'A',
        RIGHT_1 = 'D',
        STAY_1 = 'S',
        DISPOSE_1 = 'E',

        // Player 2
        UP_2 = 'I',
        DOWN_2 = 'M',
        LEFT_2 = 'J',
        RIGHT_2 = 'L',
        STAY_2 = 'K',
        DISPOSE_2 = 'O',

        // Numbers (main menu)
        _1 = '1',
        _2 = '2',
        _3 = '3',
        _4 = '4',
        _5 = '5',
        _6 = '6',
        _7 = '7',
        _8 = '8',
        _9 = '9',

        // Other
        H = 'H',
        ESC = 27,
        NONE = 0,
    };

    static bool colors;

    /** Show the manual/instructions screen (blocking). */
    static void manual();
    /** Show the "won game" screen (non-blocking). */
    static void won_game();
    /** Pause dialog that returns the mode to switch to. */
    static Mode pause(GameRunner* runner = nullptr);
    /** Main menu dialog that returns the selected mode. */
    static Mode menu(GameRunner*& runner);

    /**
     * Poll for a keypress and convert it to a Keypress enum. Returns
     * Keypress::NONE if no key is available.
     */
    static Keypress get_keypress();
};