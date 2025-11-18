#pragma once
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include "Vector.h"
#include "Console.h"
#include "Object.h"
#include "keys.h"

using std::cout, std::endl;

void gotoxy(V pos) {
	std::cout.flush();
	COORD coord;
	coord.X = pos.getX();
	coord.Y = pos.getY();
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void showCursor(bool show) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(hStdOut, &curInfo);
	curInfo.bVisible = show; // Set to TRUE to make it visible
	SetConsoleCursorInfo(hStdOut, &curInfo);
}

void cls() {
	system("cls");
}

class DownWriter {
	V startPos;
	V curPos;
public:
	DownWriter(V _startPos) : startPos(_startPos), curPos(_startPos) {}
	void writeline(const std::string& line) {
		gotoxy(curPos);
		std::cout << line;
		curPos.setY(curPos.getY() + 1);
	}
};

namespace ConsoleView {
	void init() {
		cls();
		showCursor(false);
	}

	void deinit() {
		cls();
		showCursor(true);
	}

	void drawAt(V pos, V size, const MapObject* obj) {
		if (size == V() || pos == V()) return;
		char glyph = obj ? obj->getGlyph() : ' ';
		gotoxy(pos);
		for (int y = 0; y < size.getY(); y++) {
			for (int x = 0; x < size.getX(); x++) {
				std::cout << glyph;
			}
			gotoxy(V(pos.getX(), pos.getY() + y + 1));
		}
	}

	void pause() {
		cls();
		gotoxy(V(10, 5));
		std::cout << "Game paused, press ESC again to continue or X to go back to the main menu" << std::endl;
	}

	void menu() {
		ConsoleView::init();
		DownWriter w = { V(10, 5) };
		w.writeline("Welcome to the Game!");
		w.writeline("(1) Start a new game");
		w.writeline("(8) Instruction and manual");
		w.writeline("(9) Exit");
	}

	void manual() {
		ConsoleView::init();
		DownWriter w = { V(10, 5) };
		w.writeline("Instructions:");
		w.writeline("Player 1 controls: W (up), A (left), S (stay), D (right), X (down), E (dispose)");
		w.writeline("Player 2 controls: I (up), J (left), K (stay), L (right), M (down), O (dispose)");
		w.writeline("Press any key to return to the main menu...");
		_getch();
	}

	Keypress get_keypress() {
		if (_kbhit()) {
			char ch = _getch();
			return (Keypress)(toupper(ch));
		}
		return Keypress::NONE;
	}
}