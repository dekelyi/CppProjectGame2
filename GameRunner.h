#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Console.h"
#include "EventLogger.h"

#define STEPS_FILENAME "adv-world.steps"
#define LOG_FILENAME "adv-world.result"
#define NO_KEYPRESS '~'

using std::string;

/**
 * Base class for input/timing runners controlling the game loop.
 *
 * Subclasses provide keypress/input sources, playback or recording functionality.
 */
class GameRunner {
protected:
    unsigned time = 0;
public:
    virtual ~GameRunner() { this->deinit(); }
    virtual void init() {};
    virtual void deinit() {};

    /** Tick interval used for sleeping between frames. */
    virtual unsigned get_tick_time_ms() const { return 100; };
    virtual void handle_tick() { time++; };
    virtual bool should_draw_screen() const { return true; };
    virtual string get_exit_msg() const { return ""; };

    virtual ConsoleMenu::Mode get_mode(ConsoleMenu::Mode mode) const { return mode; }
    virtual ConsoleMenu::Keypress get_keypress() = 0;
    virtual string get_input() = 0;
    virtual void handle_event(Event* e) {};

    // Allow runners to provide saving capability; default is unsupported.
    virtual void save_steps(const string& filename) { throw std::runtime_error("save not supported"); }
};

/**
 * Keyboard runner that reads input from the console using ConsoleMenu helpers.
 * Also keeps an internal buffer of steps/key presses which can be saved to a file.
 */
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

    inline virtual string get_input() override {
        Console::showCursor(true);
        string str;
        std::cin >> str;
        steps_buffer += str;
        return str;
    }

    // Save the internal steps buffer to a file. Throws on error opening the file.
    inline virtual void save_steps(const string& filename = STEPS_FILENAME) override {
        std::ofstream out(filename);
        if (!out.is_open()) throw std::runtime_error("error open file");
        out << steps_buffer;
        out.close();
    }

    inline const std::string& get_steps_buffer() const { return steps_buffer; }
    inline void clear_steps_buffer() { steps_buffer.clear(); }
};

/**
 * Runner that records player input and logs events to files for replay/testing.
 */
class SavingGameRunner : public KeyboardGameRunner {
    const string steps_filename;
    const string log_filename;
    std::ofstream steps;
    std::ofstream log;
public:
    SavingGameRunner(const string& sfn = STEPS_FILENAME, const string& lfn = LOG_FILENAME)
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

    inline virtual string get_input() override {
        string str = KeyboardGameRunner::get_input();
        steps << str;
        steps.flush();
        return str;
    }

    inline virtual void handle_event(Event* e) override {
        log << time << ":: " << e->to_string() << std::endl;
        log.flush();
    }
};

/**
 * Runner that replays input from a saved steps file.
 */
class LoadedGameRunner : virtual public GameRunner {
    string filename;
    std::ifstream steps;
public:
    LoadedGameRunner(const string& _filename = STEPS_FILENAME) :filename(_filename) {};

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
    inline virtual string get_input() override {
        string str = "";
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

/**
 * Test runner that compares runtime events to an expected log file; throws on mismatch.
 */
class TestGameRunner : public LoadedGameRunner {
    const string log_filename;
    std::ifstream log;
public:
    TestGameRunner(const string& sfn = STEPS_FILENAME, const string& lfn = LOG_FILENAME)
        : LoadedGameRunner(sfn), log_filename(lfn) {
    };

    virtual void init() override {
        LoadedGameRunner::init();
        log.open(log_filename);
        if (!log.is_open()) throw std::runtime_error("error open file");
    }
    virtual void deinit() override {
        LoadedGameRunner::deinit();
        log.close();
    }

    inline virtual void handle_event(Event* e) override {
        string expected = std::to_string(time) + ":: " + e->to_string();
        string actual;
        std::getline(log, actual);
        if (expected != actual) {
            throw EventAssertionError("Test failed at time " + std::to_string(time) + ": expected '" + expected + "', got '" + actual + "'");
        }
    }

    virtual unsigned get_tick_time_ms() const override { return 0; }

    virtual bool should_draw_screen() const override { return false; }
    virtual string get_exit_msg() const override { return "All tests passed"; };
};

/**
 * Hybrid runner that replays from a steps file and, after the file ends, falls back to keyboard input.
 */
class HybridGameRunner : public LoadedGameRunner, public KeyboardGameRunner {
public:
    HybridGameRunner(const string& filename = STEPS_FILENAME)
        : LoadedGameRunner(filename) {}

    virtual void init() override {
        LoadedGameRunner::init();
        KeyboardGameRunner::init();
    }

    virtual void deinit() override {
        LoadedGameRunner::deinit();
        KeyboardGameRunner::deinit();
    }

    virtual ConsoleMenu::Mode get_mode(ConsoleMenu::Mode mode) const override {
        ConsoleMenu::Mode loadedMode = LoadedGameRunner::get_mode(mode);
        if (loadedMode == ConsoleMenu::Mode::RUNNING) return ConsoleMenu::Mode::RUNNING;
        return KeyboardGameRunner::get_mode(mode);
    }

    inline virtual ConsoleMenu::Keypress get_keypress() override {
        // If the loaded runner still has input, use it; otherwise use keyboard
        if (LoadedGameRunner::get_mode(ConsoleMenu::Mode::RUNNING) == ConsoleMenu::Mode::RUNNING) {
            return LoadedGameRunner::get_keypress();
        }
        return KeyboardGameRunner::get_keypress();
    }

    inline virtual string get_input() override {
        if (LoadedGameRunner::get_mode(ConsoleMenu::Mode::RUNNING) == ConsoleMenu::Mode::RUNNING) {
            return LoadedGameRunner::get_input();
        }
        return KeyboardGameRunner::get_input();
    }

    virtual unsigned get_tick_time_ms() const override {
        return LoadedGameRunner::get_tick_time_ms();
    }

    // Prevent Hybrid runner from saving event logs
    inline virtual void handle_event(Event* e) override {}

	using KeyboardGameRunner::save_steps;
};