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

static BOOL SetConsoleSize(int cols, int rows) {
	HWND hWnd;
	HANDLE hConOut;
	CONSOLE_FONT_INFO fi;
	CONSOLE_SCREEN_BUFFER_INFO bi;
	int w, h, bw, bh;
	RECT rect = { 0, 0, 0, 0 };
	COORD coord = { 0, 0 };
	hWnd = GetConsoleWindow();
	if (hWnd) {
		hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hConOut && hConOut != (HANDLE)-1) {
			if (GetCurrentConsoleFont(hConOut, FALSE, &fi)) {
				if (GetClientRect(hWnd, &rect)) {
					w = rect.right - rect.left;
					h = rect.bottom - rect.top;
					if (GetWindowRect(hWnd, &rect)) {
						bw = rect.right - rect.left - w;
						bh = rect.bottom - rect.top - h;
						if (GetConsoleScreenBufferInfo(hConOut, &bi)) {
							coord.X = bi.dwSize.X;
							coord.Y = bi.dwSize.Y;
							if (coord.X < cols || coord.Y < rows) {
								if (coord.X < cols) {
									coord.X = cols;
								}
								if (coord.Y < rows) {
									coord.Y = rows;
								}
								if (!SetConsoleScreenBufferSize(hConOut, coord)) {
									return FALSE;
								}
							}
							return SetWindowPos(hWnd, NULL, rect.left, rect.top, cols * fi.dwFontSize.X + bw, rows * fi.dwFontSize.Y + bh, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
						}
					}
				}
			}
		}
	}
	return FALSE;
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
		gotoxy(V(10, 5));
		std::cout << "Welcome to the Game!" << std::endl;
		std::cout << "(1) Start a new game" << std::endl;
		std::cout << "(8) Instruction and manual" << std::endl;
		std::cout << "(9) Exit" << std::endl;
	}

	void manual() {
		ConsoleView::init();
		gotoxy(V(5, 5));
		std::cout << "Instructions:" << std::endl;
		std::cout << "Player 1 controls: W (up), A (left), S (stay), D (right), X (down), E (dispose)" << std::endl;
		std::cout << "Player 2 controls: I (up), J (left), K (stay), L (right), M (down), O (dispose)" << std::endl;
		std::cout << "Press any key to return to the main menu..." << std::endl;
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