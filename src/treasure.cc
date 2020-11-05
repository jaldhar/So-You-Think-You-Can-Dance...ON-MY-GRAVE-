// Treasure -- Treasure in S.Y.T.Y.C.D...O.M.G? (implementation)
//
#include "treasure.h"

struct Treasure::TreasureImpl {
    TreasureImpl();
    ~TreasureImpl()=default;

    int amount_;
};

Treasure::Treasure() : Item("some", "treasure", ITEMTYPE::TREASURE),
    impl_ { new Treasure::TreasureImpl() } {
}

Treasure::~Treasure()=default;

int Treasure::amount() const {
    return impl_->amount_;
}

void Treasure::setAmount(int amount) {
    impl_->amount_ = amount;
}

Treasure::TreasureImpl::TreasureImpl() : amount_{1} {
}
