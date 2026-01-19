#pragma once

/**
* Represent symbols of game element used in map files and rendering.
*/
enum class ObjType : char {
	PLAYER_1 = '$',
	PLAYER_2 = '&',
	WALL = 'W',
	DOOR = 'D',
	OBSTCALE = '*',
	KEY = 'K',
	SPRING = '#',
	SWITCH_OFF = '\\',
	SWITCH_ON = '/',
	TORCH = '!',
	RIDDLE = '?',
	BOMB = '@'
};