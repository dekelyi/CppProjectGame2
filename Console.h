/**
* Console view helpers and drawers
*/
#pragma once
#include <string>
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

/**
Sleep for ms miliseconds
*/
void console_sleep(int ms);

/**
* A Writer that starts with some padding and retains it
*/
class Writer {
	V pos;
public:
	Writer(V _pos) : pos(_pos) {}
	void writeline(const std::string& line);
};

namespace ConsoleView {
	extern bool colors;

	void drawAt(V pos, const V& size, const char glyph, const std::string& atr="", const bool padding=true);

	void init();
	void deinit();
	void pause();
	void won_game();
	void menu();
	void manual();
	void draw_borders();

	Keypress get_keypress();
};