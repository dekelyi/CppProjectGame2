#pragma once
/**
* Constant values and game settings
*/

/** --- CONSTANTS ---- */
#define HUD_SPACE_TOP 5 // size of top bar HUD
#define SIZE_X 80 // Size of the game view
#define SIZE_Y 20\

#define TICK 100 // nanoseconds per 1 game tick
#define MSG_TICKS 10 // numbers of tick to display a message

#define N_MAX_PLAYERS 2 // number of players

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
};