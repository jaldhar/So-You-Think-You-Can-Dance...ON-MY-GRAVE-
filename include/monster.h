// Monster -- A Monster in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef MONSTER_H
#define MONSTER_H 1

#include <string>
#include "combat.h"
#include "item.h"
#include "itemtype.h"

class Monster: public Item, public Combat {
public:
    Monster(std::string article, std::string name, ITEMTYPE type, int health,
    int offense, int defense);
    virtual ~Monster();
};

#endif
