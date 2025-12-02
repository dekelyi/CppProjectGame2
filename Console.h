#pragma once
#include <string>
#include "Vector.h"
#include "prelude.h"

#define UNDERSCORE "\033[4m"
#define RESET "\033[0m"

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