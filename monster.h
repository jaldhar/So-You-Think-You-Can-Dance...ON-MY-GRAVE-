// Monster -- A Monster in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef MONSTER_H
#define MONSTER_H 1

#include "item.h"
#include "combat.h"
class Player;
class Tile;

class Monster: public Item, public Combat {
public:
    Monster();
    Monster(char symbol, std::string name, std::string article,
    int health, int offense, int defense);
    Monster* clone();
    char symbol();
private:
    char _symbol;
};

#endif
