// Shield -- A Shield in S.Y.T.Y.C.D...O.M.G? (implementation)
//

using namespace std;

#include "player.h"
#include "tile.h"
#include "shield.h"

Shield::Shield() : Item() {
    _name         = "shield";
    _article      = "a";
    _offensebonus = 0;
    _defensebonus = 0;
}

Shield::Shield(string name, string article, int offensebonus, int defensebonus)
: Item(name, article) {
    _offensebonus = offensebonus;
    _defensebonus = defensebonus;
}

Shield* Shield::clone() {
    return new Shield(*this);
}

bool Shield::take(Player&p, Tile& r) {
    Shield* temp = p.shield();

    if(temp) {
        r.setContents(temp);
        p.setDefense(-(_defensebonus));
        p.setOffense(-(_offensebonus));
    }
    p.setShield(this);
    p.setDefense(_defensebonus);
    p.setOffense(_offensebonus);

    return true;
}
