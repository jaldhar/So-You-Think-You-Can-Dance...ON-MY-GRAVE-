// Weapon -- A Weapon in S.Y.T.Y.C.D...O.M.G? (implementation)
//

#include "weapon.h"

Weapon::Weapon(std::string article, std::string name, ITEMTYPE type,
    int offenseBonus, int defenseBonus) :
    Item(article, name, type), Armament(offenseBonus, defenseBonus) {
}

Weapon::~Weapon()=default;
