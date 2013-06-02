// Shield -- A Shield in S.Y.T.Y.C.D...O.M.G? (implementation)
//

using namespace std;

#include "player.h"
#include "shield.h"
#include "view.h"

extern Player player;

Shield::Shield(string article, string name, ITEMTYPE type,
    int offenseBonus, int defenseBonus) :
    Item(article, name, type), Armament(offenseBonus, defenseBonus) {
}

Shield::~Shield()=default;
