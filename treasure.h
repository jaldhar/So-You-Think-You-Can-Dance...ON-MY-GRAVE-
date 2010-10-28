// Treasure -- Treasure in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef TREASURE_H
#define TREASURE_H 1

#include "item.h"
#include "player.h"
#include "tile.h"
#include "take.h"

class Treasure: public Item, public virtual Take {
public:
    Treasure();
    Treasure*    clone();
    bool take(Player& p, Tile& r);
};

#endif
