// Weapon -- A Weapon in S.Y.T.Y.C.D...O.M.G (interface)
//

#ifndef WEAPON_H
#define WEAPON_H 1

#include "item.h"
#include "take.h"

class Weapon: public Item, public virtual Take {
public:
    Weapon();
    Weapon(std::string name, std::string article, int offensebonus, int defensebonus);
    Weapon*         clone();
    bool            take(Player&p, Tile& r);
private:
    int _offensebonus;
    int _defensebonus;
};

#endif
