/**
* Console view helpers and drawers
*/
#pragma once
#include <string>
#include <functional>
#include "Vector.h"
#include "prelude.h"

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

/**
* Console - static helper class that replaces previous free functions.
*/
class Console {
public:
    static void init();
    static void deinit();

    static void gotoxy(V pos);
    static void showCursor(bool show);
    static void cls();
    static void sleep(int ms);
};

/**
* A Writer that starts with some padding and retains it
*/
class Writer {
    V pos;
public:
    Writer(V _pos) : pos(_pos) {}
    void writeline(const std::string& line);
};

/**
* ConsoleMenu - converted from namespace to static class.
*/
class ConsoleMenu {
public:
    static bool colors;

    static void manual();
    static void won_game();
    static Mode pause();
    static Mode menu();

    static void main_loop(std::function<void(GameView*)> init);

    static Keypress get_keypress();
};