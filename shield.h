// Shield -- A Shield in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef SHIELD_H
#define SHIELD_H 1

#include "item.h"
#include "take.h"

class Shield: public Item, public virtual Take {
public:
    Shield();
    Shield(std::string name, std::string article, int offensebonus, int defensebonus);
    Shield*         clone();
    bool            take(Player&p, Tile& r);
private:
    int _offensebonus;
    int _defensebonus;
};

#endif
