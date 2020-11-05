// Player -- The Player in S.Y.T.Y.C.D...O.M.G? (implementation)
//

#include <cstdlib>
#include <memory>

#include "player.h"

struct Player::PlayerImpl {
    PlayerImpl();
    ~PlayerImpl()=default;

    int                     facingX_;
    int                     facingY_;
    std::unique_ptr<Armor>  armor_;
    int                     potions_;
    std::unique_ptr<Shield> shield_;
    int                     treasure_;
    std::unique_ptr<Weapon> weapon_;
} Player::impl_;

Player::Player() : Combat(17, 1, 1) {
}

Player::~Player()=default;

int Player::facingX() const {
    return impl_.facingX_;
}

void Player::setFacingX(int  x) {
    impl_.facingX_ = x;
}

int Player::facingY() const {
    return impl_.facingY_;
}

void Player::setFacingY(int  y) {
    impl_.facingY_ = y;
}

Armor* Player::armor() const {
    return impl_.armor_.get();
}

void Player::setArmor(Armor* a) {
    impl_.armor_.reset(a);
}

int Player::potions() const {
    return impl_.potions_;
}

void Player::setPotions(int potions) {
    impl_.potions_ += potions;
}

Shield* Player::shield() const {
    return impl_.shield_.get();
}

void Player::setShield(Shield* s) {
    impl_.shield_.reset(s);
}

int Player::treasure() const {
    return impl_.treasure_;
}

void Player::setTreasure(int amount) {
    impl_.treasure_ += amount;
}

Weapon* Player::weapon() const {
    return impl_.weapon_.get();
}

void Player::setWeapon(Weapon* w) {
    impl_.weapon_.reset(w);
}

Player::PlayerImpl::PlayerImpl() : facingX_{0}, facingY_{0}, armor_{nullptr},
potions_{0}, shield_{nullptr}, treasure_{0}, weapon_{nullptr} {
}
