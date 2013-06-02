// Monster -- A Monster in S.Y.T.Y.C.D...O.M.G? (implementation)
//

using namespace std;

#include "monster.h"

Monster::Monster(string article, string name, ITEMTYPE type, int health,
    int offense, int defense) :
    Item(article, name, type), Combat(health, offense, defense) {
}

Monster::~Monster()=default;

