#pragma once
#include <vector>
#include "BaseProp.h"
#include "player.h"

using std::vector;

class PlayersProp : public BaseProp {
public:
    /** Return a vector of player pointers currently in the room. */
    vector<Player*> get_players() const;

    /** Return number of players currently in the room. */
    inline int count_players() const {
        return (int)get_players().size();
    };

    /** Return other objects that belong to players (e.g. collectibles). */
    vector<MapObject*> get_objects() const;

    /** Attempt to remove object `obj`; if it is a Player, handle lives and respawn. */
    bool remove_object(MapObject* obj);
};