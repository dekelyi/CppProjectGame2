#pragma once

/**
* Represent symbols of game element
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