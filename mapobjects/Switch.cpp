#include "Switch.h"
#include "../types/Player.h"
#include "../Room.h"

void Switch::setSwitch(GameRoom* room, bool val) {
    if (val == switched) return;
    room->clear(*this);
    switched = val;
    if (callback) callback(switched);
    setGlyph(val ? (char)ObjType::SWITCH_ON : (char)ObjType::SWITCH_OFF);
    room->runner->handle_event(new SwitchToggled(this, switched));
}

MapObject::M_CODE Switch::handle_collision(GameRoom* room, MapObject* other, Move& move) {
    switchers.push_back({ other, other->getPosition() });
    setSwitch(room, true);
    return M_CODE::CANT_MOVE;
}

void Switch::handle_tick(GameRoom* room) {
    MapObject::handle_tick(room);
    if (switched) {
        switchers.remove_if([](const Switcher& s) { return s.pos != s.obj->getPosition(); });
        if (switchers.empty()) setSwitch(room, false);
    }
}
