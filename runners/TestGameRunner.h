#pragma once
#include "LoadedThenExitGameRunner.h"
#include <fstream>

class TestGameRunner : public LoadedThenExitGameRunner {
    const std::string log_filename;
    std::ifstream log;
public:
    TestGameRunner(const std::string& sfn = STEPS_FILENAME, const std::string& lfn = LOG_FILENAME)
        : LoadedThenExitGameRunner(sfn), log_filename(lfn) {
    };

    virtual void init() override {
        LoadedThenExitGameRunner::init();
        log.open(log_filename);
        if (!log.is_open()) throw std::runtime_error("error open file");
    }
    virtual void deinit() override {
        LoadedThenExitGameRunner::deinit();
        log.close();
    }

    inline virtual void handle_event(Event* e) override {
        std::string expected = std::to_string(time) + ":: " + e->to_string();
        std::string actual;
        std::getline(log, actual);
        if (expected != actual) {
            throw EventAssertionError("Test failed at time " + std::to_string(time) + ": expected '" + expected + "', got '" + actual + "'");
        }
    }

    virtual unsigned get_tick_time_ms() const override { return 0; }

    virtual bool should_draw_screen() const override { return false; }
    virtual std::string get_exit_msg() const override { return "All tests passed"; };
};
