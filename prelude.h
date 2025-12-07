#pragma once

#define SIZE_X 80
#define SIZE_Y 20
#define HUD_SPACE_TOP 5

#define TICK 100
#define MSG_TICKS 10

#define N_MAX_PLAYERS 2

enum class Mode {
	EXIT = 0,
	RUNNING,
	PAUSED,
	CONTINUE,
	MENU,
	WINNING,
};

enum class Keypress {
	UP_1 = 'W',
	DOWN_1 = 'X',
	LEFT_1 = 'A',
	RIGHT_1 = 'D',
	STAY_1 = 'S',
	DISPOSE_1 = 'E',

	UP_2 = 'I',
	DOWN_2 = 'M',
	LEFT_2 = 'J',
	RIGHT_2 = 'L',
	STAY_2 = 'K',
	DISPOSE_2 = 'O',

	_1 = '1',
	_7 = '7',
	_8 = '8',
	_9 = '9',

	H = 'H',
	ESC = 27,
	NONE = 0,
};

enum class ObjType : char {
	PLAYER_1 = '$',
	PLAYER_2 = '&',
	WALL = 'W',
	OBSTCALE = '*',
	KEY = 'K',
	SPRING = '#'
};