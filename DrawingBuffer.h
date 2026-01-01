#pragma once
#include <string>
#include <array>
#include <iostream>
#include "Vector.h"
#include "Console.h"

using std::array, std::string;

struct DrawingObject {
	char glyph;
	std::string attr;
};

static const DrawingObject DNULL = { ' ', "" };

template <unsigned int X, unsigned int Y>
class MapBuffer {
	array<array<DrawingObject, X>, Y> buffer;
	unsigned int padding_top = HUD_SPACE_TOP;
public:
	MapBuffer() {
		for (int y = 0; y < Y; y++)
			for (int x = 0; x < X; x++)
				buffer[y][x] = DNULL;
	}
	DrawingObject get_at(V pos) const {
		int x = pos.getX(), y = pos.getY();
		if (x < 0 || y < 0 || x >= X || y >= Y)
			return DNULL;
		return buffer[y][x];
	}
	void set_at(V pos, DrawingObject obj) {
		int x = pos.getX(), y = pos.getY();
		if (x < 0 || y < 0 || x >= X || y >= Y)
			return;
		buffer[y][x] = obj;
	}

	void set_at(V pos, V size, const DrawingObject& obj) {
		unsigned int x = pos.getX(), y = pos.getY(), size_x = size.getX(), size_y = size.getY();
		for (unsigned int dy = 0; dy < size_y; dy++)
			for (unsigned int dx = 0; dx < size_x; dx++)
				set_at(V(x + dx, y + dy), obj);
	}

	void draw() const {
		gotoxy(V(0, padding_top));
		for (unsigned int y = 0; y < Y; y++) {
			for (unsigned int x = 0; x < X; x++) {
				const DrawingObject& obj = buffer[y][x];
				std::cout << obj.attr << obj.glyph << A_RESET;
			}
			std::cout << std::endl;
		}
	}
};