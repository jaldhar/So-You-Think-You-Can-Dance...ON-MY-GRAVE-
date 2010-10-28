// Player -- The Player in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef PLAYER_H
#define PLAYER_H 1

#include "armor.h"
#include "combat.h"
#include "shield.h"
#include "weapon.h"

class Player: public Combat {
public:
    Player();
    Player* clone();
    Armor*  armor();
    void setArmor(Armor* a);
    int  potions();
    void setPotions(int n);
    Shield*  shield();
    void setShield(Shield* s);
    int  treasure();
    void setTreasure(int n);
    Weapon*  weapon();
    void setWeapon(Weapon* w);
protected:
    int     _potions;
    int     _treasure;
    Weapon* _weapon;
    Shield* _shield;
    Armor*  _armor;
};

#endif
