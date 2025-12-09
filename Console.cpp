#pragma once
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <format>
#include "Vector.h"
#include "Console.h"
#include "prelude.h"

using std::cout, std::endl, std::string, std::format;

// --------- INTERNAL FUNCTIONS --------------------
/**
goto a position in the screen
*/
void gotoxy(V pos) {
	cout.flush();
	COORD coord;
	coord.X = pos.getX();
	coord.Y = pos.getY();
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/**
Show or hide curesor
*/
void showCursor(bool show) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(hStdOut, &curInfo);
	curInfo.bVisible = show; // Set to TRUE to make it visible
	SetConsoleCursorInfo(hStdOut, &curInfo);
}

/**
clear the screen
*/
void cls() {
	system("cls");
}

// --------- EXPORT FUNCTIONS -----------------

/**
Sleep for ms miliseconds
*/
void console_sleep(int ms) {
	Sleep(ms);
}

// Writer
void Writer::writeline(const string& line) {
	gotoxy(pos);
	cout << line;
	pos.setY(pos.getY() + 1);
}

namespace ConsoleView {
	bool colors = true;

	void init() {
		cls();
		showCursor(false);

		// Enable ANSI escape codes
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD dwMode = 0;
		GetConsoleMode(hStdOut, &dwMode);
		dwMode |= ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hStdOut, dwMode);
	}

	void deinit() {
		cls();
		showCursor(true);
	}

	void drawAt(V pos, const V& size, const char glyph, const string& atr = "", const bool padding = true) {
		if (size == V()) return;
		if (padding) pos = pos + V(0, HUD_SPACE_TOP); // HUD padding
		cout << atr << endl;
		gotoxy(pos);
		for (int y = 0; y < size.getY(); y++) {
			for (int x = 0; x < size.getX(); x++) {
				cout << glyph;
			}
			gotoxy(pos + V(0, y + 1));
		}
		cout << A_RESET << endl;
	}

	void pause() {
		cls();
		gotoxy(V(10, 5));
		cout << "Game paused, press ESC again to continue or H to go back to the main menu" << endl;
	}

	void won_game() {
		ConsoleView::init();
		Writer w = { V(10, 5) };
		w.writeline("You won the Game");
		w.writeline("press any key to return to the main menu");
	}

	void menu() {
		ConsoleView::init();
		Writer w = { V(10, 5) };
		w.writeline("Welcome to the Game!");
		w.writeline("(1) Start a new game");
		w.writeline(format("(7) Colors: turn {}", colors ? "off" : "on"));
		w.writeline("(8) Instruction and manual");
		w.writeline("(9) Exit");
	}

	void manual() {
		ConsoleView::init();
		Writer w = { V(10, 5) };
		w.writeline("Instructions:");
		w.writeline("Player 1 controls: W (up), A (left), S (stay), D (right), X (down), E (dispose)");
		w.writeline("Player 2 controls: I (up), J (left), K (stay), L (right), M (down), O (dispose)");
		w.writeline("Press any key to return to the main menu...");
		_getch();
	}

	void draw_borders() {
		ConsoleView::drawAt(V(0, HUD_SPACE_TOP - 1), V(SIZE_X, 1), ' ', A_UNDERSCORE, false);
		ConsoleView::drawAt(V(0, SIZE_Y), V(SIZE_X, 1), '-');
		ConsoleView::drawAt(V(SIZE_X, 0), V(1, SIZE_Y), '|');
	}

	Keypress get_keypress() {
		if (_kbhit()) {
			char ch = _getch();
			return (Keypress)(toupper(ch));
		}
		return Keypress::NONE;
	}
}