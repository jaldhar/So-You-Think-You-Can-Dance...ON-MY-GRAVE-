// Treasure -- Treasure in S.Y.T.Y.C.D...O.M.G? (implementation)
//

using namespace std;

#include "treasure.h"

Treasure::Treasure() : Item() {
    _name =    "treasure";
    _article = "some";
}

Treasure* Treasure::clone() {
    return new Treasure(*this);
}

bool Treasure::take(Player&p, Tile& r) {
    p.setTreasure(1);
    r.setContents(0);

    return true;
}

