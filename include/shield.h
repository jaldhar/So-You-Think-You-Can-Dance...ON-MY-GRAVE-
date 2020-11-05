// Shield -- A Shield in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef SHIELD_H
#define SHIELD_H 1

#include <string>
#include "armament.h"
#include "item.h"

class Shield: public Item, public Armament {
public:
    Shield(std::string article, std::string name, ITEMTYPE type,
    int offensebonus, int defensebonus);
    virtual ~Shield();
};

#endif
