// Potion -- A potion in S.Y.T.Y.C.D...O.M.G? (implementation)
//

using namespace std;

#include "player.h"
#include "potion.h"

extern Player player;

struct Potion::PotionImpl {
    PotionImpl(int number);
    ~PotionImpl()=default;
    int _number;
};

Potion::Potion(int number) : Item("a", "potion", ITEMTYPE::POTION),
    _impl { new Potion::PotionImpl(number) } {
}

Potion::~Potion()=default;

int Potion::number() const {
    return _impl->_number;
}

void Potion::setNumber(int number) {
    _impl->_number = number;
}

Potion::PotionImpl::PotionImpl(int number) {
    _number = number;
}
