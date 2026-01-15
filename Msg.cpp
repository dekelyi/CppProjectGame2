#include "Msg.h"
#include "GameView.h"

using std::cin;

void MsgWithInput::handle_tick(GameView& game) {
	if (!active)
		Msg::handle_tick(game);
	else { // wait for input
		on_input(game.runner->get_input());
	}
}