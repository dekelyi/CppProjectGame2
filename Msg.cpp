#include "Msg.h"
#include "GameView.h"

using std::cin;

/**
 * MsgWithInput: waits for user input via the GameRunner. Calls `on_input`
 * when input is available; otherwise falls back to the base handler.
 */
void MsgWithInput::handle_tick(GameView& game) {
	if (!active)
		Msg::handle_tick(game);
	else { // wait for input
		on_input(game.runner->get_input());
	}
}