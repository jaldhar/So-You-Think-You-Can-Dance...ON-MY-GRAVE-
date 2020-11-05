// Shield -- A Shield in S.Y.T.Y.C.D...O.M.G? (implementation)
//
#include "shield.h"

Shield::Shield(std::string article, std::string name, ITEMTYPE type,
    int offenseBonus, int defenseBonus) :
    Item(article, name, type), Armament(offenseBonus, defenseBonus) {
}

Shield::~Shield()=default;
