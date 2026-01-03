#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Vector.h"
#include "Console.h"
#include <functional>

using std::vector, std::string, std::endl, std::cout;

struct DrawingObject {
	char glyph = ' ';
	std::string attr = "";

	bool operator==(const DrawingObject& other) const = default;
};

static const DrawingObject DNULL = DrawingObject();

class MapBuffer {
	vector<vector<DrawingObject>> buffer;
public:
	const int legend_y_pos = 0;
	const unsigned X;
	const unsigned Y;

	MapBuffer(unsigned x, unsigned y, int legend_pos) : X(x), Y(y), legend_y_pos(legend_pos) {
		buffer = { X, vector<DrawingObject>(Y) };
	}
	DrawingObject get_at(V pos) const {
		unsigned x = pos.getX(), y = pos.getY();
		if (x < 0 || y < 0 || x >= X || y >= Y)
			return DNULL;
		return buffer[x][y];
	}

	bool is_set(V pos) const {
		unsigned x = pos.getX(), y = pos.getY();
		if (x < 0 || y < 0 || x >= X || y >= Y)
			return false;
		return buffer[x][y] != DNULL;
	}

	void set_at(V pos, DrawingObject obj) {
		unsigned x = pos.getX(), y = pos.getY();
		if (x < 0 || y < 0 || x >= X || y >= Y)
			return;
		buffer[x][y] = obj;
	}

	void set_at(V pos, V size, const DrawingObject& obj) {
		unsigned x = pos.getX(), y = pos.getY(), size_x = size.getX(), size_y = size.getY();
		for (unsigned dy = 0; dy < size_y; dy++)
			for (unsigned dx = 0; dx < size_x; dx++)
				set_at(V(x + dx, y + dy), obj);
	}

	void draw(std::function<void(unsigned)> draw_hud) const {
		string border_h = string(X+2, CH_BLOCK_GREY);
		gotoxy(V(0, 0));
		for (unsigned y = 0; y < Y; y++) {
			if (y == legend_y_pos) draw_hud(y ? y+1 : y);
			if (y == 0) cout << border_h << endl;
			cout << CH_BLOCK_GREY;
			for (unsigned x = 0; x < X; x++) {
				const DrawingObject& obj = buffer[x][y];
				cout << obj.attr << obj.glyph << A_RESET;
			}
			cout << CH_BLOCK_GREY << endl;
		}
		cout << border_h << endl;
		if (legend_y_pos >= Y) draw_hud(Y+2);
	}
};