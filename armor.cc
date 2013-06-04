// Armor -- A suit of armor in S.Y.T.Y.C.D...O.M.G? (implementation)
//

using namespace std;

#include "armor.h"
#include "view.h"

Armor::Armor(string article, string name, ITEMTYPE type,
    int offenseBonus, int defenseBonus) :
    Item(article, name, type), Armament(offenseBonus, defenseBonus) {
}

Armor::~Armor()=default;
