#pragma once
#include <string>

using std::string;
class GameView;

/**
 * A message on the screen
 *
 * Tracks message text and remaining ticks to display.
 */
struct Msg {
    string text;
    int ticks_left;

    Msg() : text(""), ticks_left(0) {}
    Msg(const string& t, int ticks) : text(t), ticks_left(ticks) {}

    inline string getText() const { return text; }

    /** Called each tick to decrement lifetime. */
    inline virtual void handle_tick(GameView& game) { if (ticks_left > 0) ticks_left--; }

    inline virtual bool is_active() const { return ticks_left > 0; }
};

/**
 * A message on the screen, waiting for input. Remains active until input
 * is provided or underlying message expires.
 */
struct MsgWithInput : public Msg {
    bool active = true;
    inline virtual bool is_active() const override { return active || Msg::is_active(); }

    /** Called when the user provides input for the modal message. */
    virtual void on_input(string str) = 0;
    virtual void handle_tick(GameView& game) override;
};