// Potion -- A Potion in S.Y.T.Y.C.D...O.M.G (interface)
//

#ifndef POTION_H
#define POTION_H 1

#include "item.h"
#include "take.h"

class Potion: public Item, public virtual Take {
public:
    Potion();
    Potion(std::string name, std::string article);
    Potion*         clone();
    bool            take(Player&p, Tile& r);
};

#endif
