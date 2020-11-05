// Treasure -- Treasure in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef TREASURE_H
#define TREASURE_H 1

#include <memory>
#include "item.h"

class Treasure: public Item {
public:
    Treasure();
    virtual ~Treasure();
    int  amount() const;
    void setAmount(int amount);

private:
    struct TreasureImpl;
    std::unique_ptr<TreasureImpl> impl_;
};

#endif
