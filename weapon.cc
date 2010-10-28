// Weapon -- A Weapon in S.Y.T.Y.C.D...O.M.G? (implementation)
//

using namespace std;

#include "player.h"
#include "tile.h"
#include "weapon.h"

Weapon::Weapon() : Item() {
    _name         = "weapon";
    _article      = "a";
    _offensebonus = 0;
    _defensebonus = 0;
}

Weapon::Weapon(string name, string article, int offensebonus, int defensebonus)
: Item(name, article) {
    _offensebonus = offensebonus;
    _defensebonus = defensebonus;
}

Weapon* Weapon::clone() {
    return new Weapon(*this);
}

bool Weapon::take(Player&p, Tile& r) {
    Weapon* temp = p.weapon();

    if(temp) {
        r.setContents(temp);
        p.setDefense(-(_defensebonus));
        p.setOffense(-(_offensebonus));
    }
    p.setWeapon(this);
    p.setDefense(_defensebonus);
    p.setOffense(_offensebonus);

    return true;
}
