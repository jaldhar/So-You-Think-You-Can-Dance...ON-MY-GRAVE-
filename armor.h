// Armor -- Armor in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef ARMOR_H
#define ARMOR_H 1

#include "item.h"
#include "take.h"

class Armor: public Item, public virtual Take {
public:
    Armor();
    Armor(std::string name, std::string article, int offensebonus, int defensebonus);
    Armor *         clone();
    bool            take(Player&p, Tile& r);
private:
    int _offensebonus;
    int _defensebonus;
};

#endif
