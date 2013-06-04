// Weapon -- A Weapon in S.Y.T.Y.C.D...O.M.G? (implementation)
//

using namespace std;

#include "weapon.h"
#include "view.h"

Weapon::Weapon(string article, string name, ITEMTYPE type,
    int offenseBonus, int defenseBonus) :
    Item(article, name, type), Armament(offenseBonus, defenseBonus) {
}

Weapon::~Weapon()=default;
