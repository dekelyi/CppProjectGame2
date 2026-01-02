#pragma once
#include "Object.h"

#define BOMB_AREA_1 1
#define BOMB_AREA_2 3
#define BOMB_TIMER 5

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

	virtual void handle_tick(GameRoom* room) override;
};


class Bomb : public Collectible {
public:
    int bomb_timer = -1;

    Bomb(V _pos) : Collectible(_pos, V(1, 1), (char)ObjType::BOMB) { attr = A_FOREGROUND_RED; }

    inline virtual void handle_tick(GameRoom* room) override {
        MapObject::handle_tick(room);
        if (bomb_timer == 0) {
            do_bomb(room);
            bomb_timer = -1;
        }
        else if (bomb_timer > 0) bomb_timer--;
    }

    void do_bomb(GameRoom* room) const;
    virtual void handle_dump(GameRoom* room) override;
};