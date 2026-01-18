#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "prelude.h"
#include "Console.h"
#include "EventLogger.h"

#define FILENAME "adv-world.steps"
#define NO_KEYPRESS '~'

using std::string;

class GameRunner {
	unsigned time = 0;
public:
	~GameRunner() { deinit(); }
	virtual void init() {};
	virtual void deinit() {};

	virtual Mode get_mode(Mode mode) const { return mode; }
	virtual Keypress get_keypress() = 0;
	virtual string get_input() = 0;
	virtual void handle_event(Event* e) {};
};

class KeyboardGameRunner : virtual public GameRunner {
public:
	inline virtual Keypress get_keypress() override {
		return ConsoleMenu::get_keypress();
	}

	inline virtual string get_input() override {
		Console::showCursor(true);
		string str;
		std::cin >> str;
		return str;
	}
};

class SavingGameRunner : public KeyboardGameRunner {
	std::string filename;
	std::ofstream file;
public:
	SavingGameRunner(const std::string& _filename = FILENAME) :filename(_filename) {};

	virtual void init() override {
		file.open(filename);
		if (!file.is_open()) throw std::runtime_error("error open file");
	}
	virtual void deinit() override {
		file.close();
	}

	inline virtual Keypress get_keypress() override {
		Keypress e = KeyboardGameRunner::get_keypress();
		if (e == Keypress::ESC) return e;
		char ch = (bool)e ? (char)e : NO_KEYPRESS;
		file << ch;
		file.flush();
		return e;
	};

	inline virtual string get_input() override {
		string str = KeyboardGameRunner::get_input();
		file << str;
		file.flush();
		return str;
	}
};

class LoadedGameRunner : public GameRunner {
	std::string filename;
	std::ifstream file;
public:
	LoadedGameRunner(const std::string& _filename = FILENAME) :filename(_filename) {};

	virtual void init() override {
		file.open(filename);
		if (!file.is_open()) throw std::runtime_error("error open file");
	}
	virtual void deinit() override {
		file.close();
	}

	virtual Mode get_mode(Mode mode) const override {
		if (!file.eof()) {
			return Mode::RUNNING;
		}
		else {
			return Mode::EXIT;
		}
	}

	inline virtual Keypress get_keypress() override {
		char ch = 0;
		if (!file.get(ch) || ch == NO_KEYPRESS) ch = 0;
		return (Keypress)ch;
	}
	inline virtual string get_input() override {
		string str = "";
		while (file.peek() != NO_KEYPRESS) {
			char ch;
			file.get(ch);
			str += ch;
		}
		return str;
	}
};