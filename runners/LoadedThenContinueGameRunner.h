#pragma once
#include "LoadedThenExitGameRunner.h"
#include "KeyboardGameRunner.h"

class LoadedThenContinueGameRunner : public LoadedThenExitGameRunner, public KeyboardGameRunner {
public:
    LoadedThenContinueGameRunner(const std::string& filename = STEPS_FILENAME)
        : LoadedThenExitGameRunner(filename) {}

    virtual void init() override {
        LoadedThenExitGameRunner::init();
        KeyboardGameRunner::init();
    }

    virtual void deinit() override {
        LoadedThenExitGameRunner::deinit();
        KeyboardGameRunner::deinit();
    }

    virtual ConsoleMenu::Mode get_mode(ConsoleMenu::Mode mode) const override {
        ConsoleMenu::Mode loadedMode = LoadedThenExitGameRunner::get_mode(mode);
        if (loadedMode == ConsoleMenu::Mode::RUNNING) return ConsoleMenu::Mode::RUNNING;
        return KeyboardGameRunner::get_mode(mode);
    }

    inline virtual ConsoleMenu::Keypress get_keypress() override {
        if (LoadedThenExitGameRunner::get_mode(ConsoleMenu::Mode::RUNNING) == ConsoleMenu::Mode::RUNNING) {
            return LoadedThenExitGameRunner::get_keypress();
        }
        return KeyboardGameRunner::get_keypress();
    }

    inline virtual std::string get_input() override {
        if (LoadedThenExitGameRunner::get_mode(ConsoleMenu::Mode::RUNNING) == ConsoleMenu::Mode::RUNNING) {
            return LoadedThenExitGameRunner::get_input();
        }
        return KeyboardGameRunner::get_input();
    }

    virtual unsigned get_tick_time_ms() const override {
        return LoadedThenExitGameRunner::get_tick_time_ms();
    }

    inline virtual void handle_event(Event* e) override {}

    using KeyboardGameRunner::save_steps;
};
