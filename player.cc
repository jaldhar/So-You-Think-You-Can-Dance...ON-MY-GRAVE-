// Player -- The Player in S.Y.T.Y.C.D...O.M.G? (implementation)
//

#include <cstdlib>
using namespace std;

#include "player.h"
#include "weapon.h"

Player::Player() : Combat(50, 1, 1) {
    _potions  = 0;
    _treasure = 0;
    _weapon = new Weapon("knife", "a", 0, 0);
    _shield = 0;
    _armor  = 0;
}

Player* Player::clone() {
    return new Player(*this);
}

Armor* Player::armor() {
    return _armor;
}

void Player::setArmor(Armor* a) {
    if (a == 0) {
        delete _armor;
        _armor = 0;
    }
    else
        _armor = a;
}

int Player::potions() {
    return _potions;
}

void Player::setPotions(int n) {
    _potions += n;
}

Shield* Player::shield() {
    return _shield;
}

void Player::setShield(Shield* s) {
    if (s == 0) {
        delete _shield;
        _shield = 0;
    }
    else
        _shield = s;
}

int Player::treasure() {
    return _treasure;
}

void Player::setTreasure(int n) {
    _treasure += n;
}

Weapon* Player::weapon() {
    return _weapon;
}

void Player::setWeapon(Weapon* w) {
    if (w == 0) {
        delete _weapon;
        _weapon = 0;
    }
    else
        _weapon = w;
}
