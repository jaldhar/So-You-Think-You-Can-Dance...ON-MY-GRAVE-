// Armor -- A suit of armor in S.Y.T.Y.C.D...O.M.G? (implementation)
//

using namespace std;

#include "player.h"
#include "tile.h"
#include "armor.h"

Armor::Armor() : Item() {
    _name         = "armor";
    _article      = "some";
    _offensebonus = 0;
    _defensebonus = 0;
}

Armor::Armor(string name, string article, int offensebonus, int defensebonus)
: Item(name, article) {
    _offensebonus = offensebonus;
    _defensebonus = defensebonus;
}

Armor* Armor::clone() {
    return new Armor(*this);
}

bool Armor::take(Player&p, Tile& r) {
    Armor* temp = p.armor();

    if(temp) {
        r.setContents(temp);
        p.setDefense(-(_defensebonus));
        p.setOffense(-(_offensebonus));
    }
    p.setArmor(this);
    p.setDefense(_defensebonus);
    p.setOffense(_offensebonus);

    return true;
}
