#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "Console.h"
#include "Events.h"

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

// include common runners for convenience
#include "runners/KeyboardGameRunner.h"
#include "runners/SavingGameRunner.h"
#include "runners/LoadedThenExitGameRunner.h"
#include "runners/TestGameRunner.h"
#include "runners/LoadedThenContinueGameRunner.h"