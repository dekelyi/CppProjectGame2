#include <conio.h>
#include "Screen.h"
#include "Map.h"
#include "MapObj.h"
#include "player.h"

void Map::init() {
	cls();
	hideCursor();
	player1.setPosition(V(10, 10));
	this->addObject(MapObject(V(20, 20), V(10,1), (char)ObjType::WALL));
	this->addObject(player1);
}

void Map::loop() {
	hideCursor();
	while (true) {
		if (_kbhit()) {
			char ch = _getch();
			player1.handle_input(this, ch);
		}
	}
}