// Player -- The Player in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef PLAYER_H
#define PLAYER_H 1

#include "armor.h"
#include "combat.h"
#include "potion.h"
#include "treasure.h"
#include "shield.h"
#include "weapon.h"

class Player : public Combat {
public:
    Player();
    virtual ~Player();
    int                      facingX() const;
    void                     setFacingX(int x);
    int                      facingY() const;
    void                     setFacingY(int y);
    Armor*                   armor() const;
    void                     setArmor(Armor* armor);
    int                      potions() const;
    void                     setPotions(int potions);
    Shield*                  shield() const;
    void                     setShield(Shield* shield);
    int                      treasure() const;
    void                     setTreasure(int amount);
    Weapon*                  weapon() const;
    void                     setWeapon(Weapon* weapon);

private:
    struct PlayerImpl;
    static PlayerImpl impl_;
};

#endif
