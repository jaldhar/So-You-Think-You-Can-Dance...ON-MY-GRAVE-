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

unique_ptr<Armor>& Player::armor() const {
    return _impl._armor;
}

void Player::setArmor(Armor* armor) {
    Armor* temp = _impl._armor.release();
    _impl._armor.reset(armor);
    delete temp;
}

int Player::potions() const {
    return _impl._potions;
}

void Player::setPotions(int potions) {
    _impl._potions += potions;
}

unique_ptr<Shield>& Player::shield() const {
    return _impl._shield;
}

void Player::setShield(Shield* shield) {
    Shield* temp = _impl._shield.release();
    _impl._shield.reset(shield);
    delete temp;
}

int Player::treasure() const {
    return _impl._treasure;
}

void Player::setTreasure(int amount) {
    _impl._treasure += amount;
}

unique_ptr<Weapon>& Player::weapon() const {
    return _impl._weapon;
}

void Player::setWeapon(Weapon* weapon) {
    Weapon* temp = _impl._weapon.release();
    _impl._weapon.reset(weapon);
    delete temp;
}

Player::PlayerImpl::PlayerImpl() {
    _potions  = 0;
    _treasure = 0;
}
