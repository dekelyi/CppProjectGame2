#pragma once
#include <string>
#include "Vector.h"
#include "prelude.h"
#include <cstdio>

#define ANSI_ESC "\033["
#define A_FOREGROUND_RED ANSI_ESC "31m"
#define A_FOREGROUND_GREEN ANSI_ESC "32m"
#define A_FOREGROUND_YELLOW ANSI_ESC "33m"
#define A_FOREGROUND_BLUE ANSI_ESC "34m"
#define A_FOREGROUND_MAGENTA ANSI_ESC "35m"
#define A_FOREGROUND_CYAN ANSI_ESC "36m"
#define A_UNDERSCORE ANSI_ESC "4m"
#define A_BOLD ANSI_ESC "1m"
#define A_RESET ANSI_ESC "0m"

class Writer {
	V pos;
public:
	Writer(V _pos) : pos(_pos) {}
	void writeline(const std::string& line);
};

namespace ConsoleView {
	void drawAt(V pos, V size, char glyph, const std::string atr="", bool padding=true);

	void init();
	void deinit();
	void pause();
	void won_game();
	void menu();
	void manual();


	Keypress get_keypress();
};