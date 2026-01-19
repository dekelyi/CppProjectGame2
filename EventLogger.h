#pragma once
#include <string>
#include "Object.h"
#include "Riddle.h"

using std::string;

class Event {
public:
	const MapObject* actor = nullptr;
	Event(const MapObject* _actor):  actor(_actor) {};

	virtual string to_string() = 0;
};

class GameEndedEvent : public Event {
public:
	GameEndedEvent() : Event(nullptr) {};
	virtual string to_string() override {
		return "GAME ENDED";
	};
};

class IntAttrModified : public Event {
protected:
	unsigned amount;

	string to_string(const string& attr) {
		return std::format("OBJ {} MODIFIED: {} by {}", actor->getGlyph(), attr, amount);
	}
public:
	IntAttrModified(const MapObject* _actor, unsigned val)
		: amount(val), Event(_actor) {
	}
};

class LostLife : public IntAttrModified {
	using IntAttrModified::IntAttrModified;

	virtual string to_string() override {
		return IntAttrModified::to_string("LOST LIFE");
	}
};
class GainedScore : public IntAttrModified {
	using IntAttrModified::IntAttrModified;

	virtual string to_string() override {
		return IntAttrModified::to_string("GAINED SCORE");
	}
};


class EventRiddleInteracted : public Event {
public:
	const Riddle* riddle;
	const unsigned answer;

	EventRiddleInteracted(const MapObject* _actor, const Riddle* _riddle, const unsigned _answer)
		: Event(_actor), riddle(_riddle), answer(_answer) {
	};

	virtual string to_string() override {
		return std::format("OBJ {} INTERACTED WITH RIDDLE {}, ANSWER {}, CORRECT: {}",
			actor->getGlyph(), riddle->data.question,
			riddle->data.answers[answer], (answer == riddle->data.correct_answer_index) ? "yes" : "no");
	};
};

class EventAssertionError : public std::runtime_error {
	public:
		EventAssertionError(const string& msg) : std::runtime_error(msg) {}
};