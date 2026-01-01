#include "Msg.h"
#include "Console.h"
#include <iostream>

using std::cin;

void MsgWithInput::handle_tick() {
	if (!active)
		Msg::handle_tick();
	else { // wait for input
		showCursor(true);
		int n;
		cin >> n;
		on_input((char)(n + '0'));
	}
}