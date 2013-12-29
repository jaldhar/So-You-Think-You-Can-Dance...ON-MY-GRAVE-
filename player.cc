// Player -- The Player in S.Y.T.Y.C.D...O.M.G? (implementation)
//

#include <cstdlib>
#include <memory>
using namespace std;

#include "player.h"

struct Player::PlayerImpl {
    PlayerImpl();
    ~PlayerImpl()=default;

    unique_ptr<Armor>  _armor;
    int                _potions;
    unique_ptr<Shield> _shield;
    int                _treasure;
    unique_ptr<Weapon> _weapon;
} Player::_impl;

Player::Player() : Combat(17, 1, 1) {
}

Player::~Player()=default;

Armor* Player::armor() const {
    return _impl._armor.get();
}

void Player::setArmor(Armor* a) {
    _impl._armor.reset(a);
}

int Player::potions() const {
    return _impl._potions;
}

void Player::setPotions(int potions) {
    _impl._potions += potions;
}

Shield* Player::shield() const {
    return _impl._shield.get();
}

void Player::setShield(Shield* s) {
    _impl._shield.reset(s);
}

int Player::treasure() const {
    return _impl._treasure;
}

void Player::setTreasure(int amount) {
    _impl._treasure += amount;
}

Weapon* Player::weapon() const {
    return _impl._weapon.get();
}

void Player::setWeapon(Weapon* w) {
    _impl._weapon.reset(w);
}

Player::PlayerImpl::PlayerImpl() {
    _potions  = 0;
    _treasure = 0;
}
