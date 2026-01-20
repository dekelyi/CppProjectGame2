#include "Player.h"
#include "../Room.h"
#include "../EventLogger.h"

MapObject::M_CODE Player::handle_collision(GameRoom* room, MapObject* other, Move& move) {
    return MapObject::handle_collision(room, other, move);
}

void Player::dump_collectible(GameRoom* room) {
    if (!collectible) return;
    // place collectible at player's current position and add to room
    collectible->setPosition(this->getPosition());
    room->add_object(collectible);
    room->runner->handle_event(new CollectibleDumped(this, collectible));
    collectible = nullptr;
}

V Player::getNextPosition(GameRoom* room) {
    V p = this->getPosition();
    V candidates[4] = { V(p.getX() + 1, p.getY()), V(p.getX() - 1, p.getY()), V(p.getX(), p.getY() + 1), V(p.getX(), p.getY() - 1) };
    for (auto &c : candidates) {
        if (!room->is_out_of_bounds(c) && !room->get_object_at(c)) return c;
    }
    return p;
}
