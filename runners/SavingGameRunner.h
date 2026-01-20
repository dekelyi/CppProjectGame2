#pragma once
#include "KeyboardGameRunner.h"
#include <fstream>

class SavingGameRunner : public KeyboardGameRunner {
    const std::string steps_filename;
    const std::string log_filename;
    std::ofstream steps;
    std::ofstream log;
public:
    SavingGameRunner(const std::string& sfn = STEPS_FILENAME, const std::string& lfn = LOG_FILENAME)
        : steps_filename(sfn), log_filename(lfn) {
    };

    virtual void init() override {
        steps.open(steps_filename);
        if (!steps.is_open()) throw std::runtime_error("error open file");
        log.open(log_filename);
        if (!log.is_open()) throw std::runtime_error("error open file");
    }
    virtual void deinit() override {
        steps.close();
        log.close();
    }

    inline virtual ConsoleMenu::Keypress get_keypress() override {
        ConsoleMenu::Keypress e = KeyboardGameRunner::get_keypress();
        if (e == ConsoleMenu::Keypress::ESC) return e;
        char ch = (bool)e ? (char)e : NO_KEYPRESS;
        steps << ch;
        steps.flush();
        return e;
    };

    inline virtual std::string get_input() override {
        std::string str = KeyboardGameRunner::get_input();
        steps << str;
        steps.flush();
        return str;
    }

    inline virtual void handle_event(Event* e) override {
        log << time << ":: " << e->to_string() << std::endl;
        log.flush();
    }
};
