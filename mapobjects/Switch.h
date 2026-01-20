#pragma once
#include "ObjTypes.h"
#include "../Object.h"
#include "Door.h"
#include "../Room.h"
#include <functional>
#include "../Events.h"

typedef std::function<void(bool)> CallbackFn;

class SwitchToggled : public Event {
public:
    bool on;
    SwitchToggled(const MapObject* _actor, bool _on) : Event(_actor), on(_on) {}
    virtual string to_string() override {
        return std::format("OBJ {} SWITCHED: {}", actor->getGlyph(), on ? "ON" : "OFF");
    }
};

inline CallbackFn SwitchDoor(GameRoom* room, Door* door, Condition* condition_switch) {
    return [=](bool switched) {
        bool last_state = door->isLocked();
        condition_switch->collected = (int)switched;
    };
}

class Switch : public MapObject {
    bool switched = false;
    CallbackFn callback;

    struct Switcher {
        MapObject* obj;
        V pos;
    };

    std::list<Switcher> switchers;
public:
    Switch(V _pos) : MapObject(_pos, S1, (char)ObjType::SWITCH_OFF) { attr = A_FOREGROUND_YELLOW; }
    Switch(V _pos, CallbackFn cb) : Switch(_pos) {
        callback = cb;
    }

    void setSwitch(GameRoom* room, bool val);

    virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override;

    virtual void handle_tick(GameRoom* room) override;
};
