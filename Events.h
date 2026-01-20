#pragma once
#include <string>
#include <format>
#include <stdexcept>
#include "Object.h"

using std::string;

/**
 * Base class for events emitted by game objects during runtime.
 */
class Event {
public:
    const MapObject* actor = nullptr;
    Event(const MapObject* _actor):  actor(_actor) {};

    virtual string to_string() = 0;
};

/**
 * Event signifying that the game has ended.
 */
class GameEndedEvent : public Event {
public:
    GameEndedEvent() : Event(nullptr) {};
    virtual string to_string() override {
        return "GAME ENDED";
    };
};

/**
 * Base event for integer attribute modifications (score/life changes).
 */
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

class EventAssertionError : public std::runtime_error {
    public:
        EventAssertionError(const string& msg) : std::runtime_error(msg) {}
};