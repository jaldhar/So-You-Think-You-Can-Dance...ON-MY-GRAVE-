// Potion -- A Potion in S.Y.T.Y.C.D...O.M.G? (implementation)
//

#include <cstdlib>
using namespace std;

#include "player.h"
#include "potion.h"
#include "tile.h"

Potion::Potion() : Item() {
    _name =    "potion";
    _article = "a";
}

Potion::Potion(string name, string article) : Item(name, article) {
}

Potion* Potion::clone() {
    return new Potion(*this);
}

bool Potion::take(Player&p, Tile& r) {
    int number = _article == "a"
        ? 1
        : atoi(_article.c_str());
    p.setPotions(number);
    r.setContents(0);

    return true;
}

