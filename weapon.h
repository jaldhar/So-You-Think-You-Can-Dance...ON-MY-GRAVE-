// Weapon -- A Weapon in S.Y.T.Y.C.D...O.M.G (interface)
//

#ifndef WEAPON_H
#define WEAPON_H 1

#include "armament.h"
#include "item.h"

class Weapon: public Item, public Armament {
public:
    Weapon(std::string article, std::string name, ITEMTYPE type,
    int offensebonus, int defensebonus);
    virtual ~Weapon();
};

#endif
