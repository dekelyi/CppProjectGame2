#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Vector.h"
#include "Console.h"

using std::vector, std::string;

struct DrawingObject {
	char glyph = ' ';
	std::string attr = "";

	bool operator==(const DrawingObject& other) const = default;
};

static const DrawingObject DNULL = DrawingObject();

class MapBuffer {
	vector<vector<DrawingObject>> buffer;
	unsigned padding_top = HUD_SPACE_TOP;
public:
	const unsigned X;
	const unsigned Y;

	MapBuffer(unsigned x, unsigned y) : X(x), Y(y) {
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

	void draw() const {
		string str;

		gotoxy(V(0, padding_top));
		for (unsigned y = 0; y < Y; y++) {
			for (unsigned x = 0; x < X; x++) {
				const DrawingObject& obj = buffer[x][y];
				if (obj != DNULL) str += obj.attr + obj.glyph + A_RESET;
			}
			str += "\n";
		}
		std::cout << str << std::endl;
	}
};