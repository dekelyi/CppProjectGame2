#pragma once
#include <string>

using std::string;

struct Msg {
	string text;
	int ticks_left;

	Msg() : text(""), ticks_left(0) {}
	Msg(const string& t, int ticks) : text(t), ticks_left(ticks) {}

	inline string getText() const {
		return text;
	}

	inline virtual void handle_tick() {
		if (ticks_left > 0)
			ticks_left--;
	}

	inline virtual bool is_active() const {
		return ticks_left > 0;
	}
};

class MsgWithInput : public Msg {
protected:
	bool active = true;
public:
	virtual void on_input(char ch) = 0;

	virtual void handle_tick() override;
	inline virtual bool is_active() const override {
		return active || Msg::is_active();
	}
};