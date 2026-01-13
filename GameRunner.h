#pragma once
#include <map>
#include <iostream>
#include <fstream>
#include "prelude.h"
#include "Console.h"

#define FILENAME "adv-world.steps"
#define NO_KEYPRESS '~'

class GameRunner {
public:
	virtual Keypress get_keypress() = 0;
	virtual void handle_tick(Keypress e) {};
};

class KeyboardGameRunner : public GameRunner {
	inline virtual Keypress get_keypress() override {
		return ConsoleMenu::get_keypress();
	}
};

class SavingGameRunner : public KeyboardGameRunner {
	std::ofstream file;
public:
	SavingGameRunner(const std::string& filename = FILENAME) :file(filename) {
		if (!file.is_open()) {
			throw std::runtime_error("error open file");
		}
	};
	~SavingGameRunner() { file.close(); };

	inline virtual void handle_tick(Keypress e) override {
		if (e == Keypress::ESC) return;
		char ch = (bool)e ? (char)e : NO_KEYPRESS;
		file << ch;
		file.flush();
	};
};

class LoadedGameRunner : public GameRunner {
	std::ifstream file;
public:
	LoadedGameRunner(const std::string& filename = FILENAME) : file(filename) {
		if (!file.is_open()) {
			throw std::runtime_error("error open file");
		}
	};
	~LoadedGameRunner() { file.close(); };

	inline virtual Keypress get_keypress() override {
		char ch = 0;
		if (!file.get(ch) || ch == NO_KEYPRESS) ch = 0;
		return (Keypress)ch;
	}
};