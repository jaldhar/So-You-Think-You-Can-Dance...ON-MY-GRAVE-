// Treasure -- Treasure in S.Y.T.Y.C.D...O.M.G? (implementation)
//

using namespace std;

#include "player.h"
#include "treasure.h"

extern Player player;

struct Treasure::TreasureImpl {
    TreasureImpl();
    ~TreasureImpl()=default;

    int _amount;
};

Treasure::Treasure() : Item("some", "treasure", ITEMTYPE::TREASURE),
    _impl { new Treasure::TreasureImpl() } {
}

Treasure::~Treasure()=default;

int Treasure::amount() const {
    return _impl->_amount;
}

void Treasure::setAmount(int amount) {
    _impl->_amount = amount;
}

Treasure::TreasureImpl::TreasureImpl() {
    _amount = 1;
}
