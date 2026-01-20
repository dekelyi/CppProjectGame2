#pragma once
#include "ObjTypes.h"
#include "../Object.h"
#include "../EventLogger.h"

#define BOMB_NOT_SET -1

class GameRoom;

class Collectible : public MapObject {

public:
    Collectible(V _pos, V _size, char _glyph) : MapObject(_pos, _size, _glyph) { attr = A_FOREGROUND_MAGENTA;  };

    virtual M_CODE handle_collision(GameRoom* room, MapObject* other, Move& move) override;
    virtual void handle_dump(GameRoom* room) {};
};

class Key : public Collectible {
public:
    Key(V _pos, V _size) : Collectible(_pos, _size, (char)ObjType::KEY) {}
};

class Torch : public Collectible {
public:
    unsigned short area;
    Torch(V _pos, unsigned short _area) : Collectible(_pos, V(1,1), (char)ObjType::TORCH), area(_area) {}
};

class Bomb : public Collectible {
public:
    static const unsigned short BOMB_TIMER = 3, BOMB_AREA = 5;

    int bomb_timer = BOMB_NOT_SET;

    Bomb(V _pos) : Collectible(_pos, V(1, 1), (char)ObjType::BOMB) { attr = A_FOREGROUND_RED; }

    void handle_tick(GameRoom* room) override;

    void do_bomb(GameRoom* room) const;
    void handle_dump(GameRoom* room) override;
};

class CollectibleCollected : public Event {
public:
    const MapObject* collectible;
    CollectibleCollected(const MapObject* _actor, const MapObject* _collectible) : Event(_actor), collectible(_collectible) {}
    virtual string to_string() override {
        return std::format("OBJ {} COLLECTED: {}", actor->getGlyph(), collectible->getGlyph());
    }
};

class CollectibleDumped : public Event {
public:
    const MapObject* collectible;
    CollectibleDumped(const MapObject* _actor, const MapObject* _collectible) : Event(_actor), collectible(_collectible) {}
    virtual string to_string() override {
        return std::format("OBJ {} DUMPED: {}", actor->getGlyph(), collectible->getGlyph());
    }
};

class BombExploded : public Event {
public:
    const unsigned area;
    BombExploded(const MapObject* _actor, unsigned _area) : Event(_actor), area(_area) {}
    virtual string to_string() override {
        return std::format("OBJ {} EXPLODED: AREA {}", actor->getGlyph(), area);
    }
};
