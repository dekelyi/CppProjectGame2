#pragma once
#include "../GameRunner.h"
#include <fstream>

class LoadedThenExitGameRunner : virtual public GameRunner {
    std::string filename;
    std::ifstream steps;
public:
    LoadedThenExitGameRunner(const std::string& _filename = STEPS_FILENAME) :filename(_filename) {};

    virtual void init() override {
        steps.open(filename);
        if (!steps.is_open()) throw std::runtime_error("error open file");
    }
    virtual void deinit() override {
        steps.close();
    }

    virtual ConsoleMenu::Mode get_mode(ConsoleMenu::Mode mode) const override {
        if (!steps.eof()) {
            return ConsoleMenu::Mode::RUNNING;
        }
        else {
            return ConsoleMenu::Mode::EXIT;
        }
    }

    inline virtual ConsoleMenu::Keypress get_keypress() override {
        char ch = 0;
        if (!steps.get(ch) || ch == NO_KEYPRESS) ch = 0;
        return (ConsoleMenu::Keypress)ch;
    }
    inline virtual std::string get_input() override {
        std::string str = "";
        while (steps.peek() != NO_KEYPRESS) {
            char ch;
            steps.get(ch);
            str += ch;
        }
        return str;
    }

    virtual unsigned get_tick_time_ms() const override {
        return GameRunner::get_tick_time_ms()/2;
    }
};
