// Shield -- A Shield in S.Y.T.Y.C.D...O.M.G? (implementation)
//

using namespace std;

#include "shield.h"
#include "view.h"

Shield::Shield(string article, string name, ITEMTYPE type,
    int offenseBonus, int defenseBonus) :
    Item(article, name, type), Armament(offenseBonus, defenseBonus) {
}

Shield::~Shield()=default;
