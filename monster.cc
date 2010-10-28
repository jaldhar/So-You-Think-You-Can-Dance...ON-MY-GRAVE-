// Monster -- A Monster in S.Y.T.Y.C.D...O.M.G? (implementation)
//

using namespace std;

#include "combat.h"
#include "monster.h"

Monster::Monster() : Item(), Combat() {
}

Monster::Monster(char symbol = 'm', string name = "monster", string article = "a", 
    int health = 1,int offense = 1, int defense = 0) :
    Item(name, article),
    Combat(health, offense, defense) {
    _symbol = symbol;
}

Monster* Monster::clone() {
    return new Monster(*this);
}

char Monster::symbol() {
    return _symbol;
}
