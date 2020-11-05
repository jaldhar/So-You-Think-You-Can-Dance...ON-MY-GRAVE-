// Potion -- A Potion in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef POTION_H
#define POTION_H 1

#include <memory>
#include "item.h"

class Potion: public Item {
public:
    Potion(int number);
    virtual ~Potion();
    int number() const;
    void setNumber(int number);

private:
    struct PotionImpl;
    std::unique_ptr<PotionImpl> impl_;
};

#endif
