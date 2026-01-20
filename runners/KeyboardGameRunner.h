#pragma once
#include "../GameRunner.h"
#include <string>
#include <iostream>
#include <fstream>

class KeyboardGameRunner : virtual public GameRunner {
protected:
    std::string steps_buffer; // internal buffer for recorded steps
public:
    inline virtual ConsoleMenu::Keypress get_keypress() override {
        ConsoleMenu::Keypress e = ConsoleMenu::get_keypress();
        if (e == ConsoleMenu::Keypress::ESC) return e;
        char ch = (bool)e ? (char)e : NO_KEYPRESS;
        steps_buffer.push_back(ch);
        return e;
    }

    inline virtual std::string get_input() override {
        Console::showCursor(true);
        std::string str;
        std::cin >> str;
        steps_buffer += str;
        return str;
    }

    inline virtual void save_steps(const std::string& filename = STEPS_FILENAME) override {
        std::ofstream out(filename);
        if (!out.is_open()) throw std::runtime_error("error open file");
        out << steps_buffer;
        out.close();
    }

    inline const std::string& get_steps_buffer() const { return steps_buffer; }
    inline void clear_steps_buffer() { steps_buffer.clear(); }
};
