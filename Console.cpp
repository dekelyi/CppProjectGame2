#pragma once
#include <iostream>
#include <windows.h> // Required for Windows API console functions
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
		if (size == V()) return;
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

	Keypress get_keypress() {
		if (_kbhit()) {
			char ch = _getch();
			return (Keypress)(toupper(ch));
		}
		return Keypress::NONE;
	}
}