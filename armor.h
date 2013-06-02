// Armor -- Armor in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef ARMOR_H
#define ARMOR_H 1

#include "armament.h"
#include "item.h"

class Armor: public Item, public Armament {
public:
    Armor(std::string article, std::string name, ITEMTYPE type,
    int offensebonus, int defensebonus);
    virtual ~Armor();
};

#endif
