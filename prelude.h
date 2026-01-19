#pragma once

/**
* Represent the current runner mode
*/
enum class Mode {
	EXIT = 0,
	RUNNING,
	PAUSED,
	CONTINUE,
	MENU,
	WINNING,
};

/**
* Represent a keypress
*/
enum class Keypress {
	// Player 1
	UP_1 = 'W',
	DOWN_1 = 'X',
	LEFT_1 = 'A',
	RIGHT_1 = 'D',
	STAY_1 = 'S',
	DISPOSE_1 = 'E',

	// Player 2
	UP_2 = 'I',
	DOWN_2 = 'M',
	LEFT_2 = 'J',
	RIGHT_2 = 'L',
	STAY_2 = 'K',
	DISPOSE_2 = 'O',

	// Numbers (main menu)
	_1 = '1',
	_7 = '7',
	_8 = '8',
	_9 = '9',

	// Other
	H = 'H',
	ESC = 27,
	NONE = 0,
};

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