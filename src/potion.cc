// Potion -- A potion in S.Y.T.Y.C.D...O.M.G? (implementation)
//
#include "potion.h"

struct Potion::PotionImpl {
    PotionImpl(int number);
    ~PotionImpl()=default;
    int number_;
};

Potion::Potion(int number) : Item("a", "potion", ITEMTYPE::POTION),
    impl_ { new Potion::PotionImpl(number) } {
}

Potion::~Potion()=default;

int Potion::number() const {
    return impl_->number_;
}

void Potion::setNumber(int number) {
    impl_->number_ = number;
}

Potion::PotionImpl::PotionImpl(int number) : number_{number} {
}
