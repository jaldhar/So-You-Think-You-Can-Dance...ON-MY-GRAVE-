// Take -- Abstract base class in S.Y.T.Y.C.D...O.M.G? for objects that can be
// taken.
//

#ifndef TAKE_H
#define TAKE_H 1

class Player;
class Tile;

struct Take {
    virtual ~Take() { };
    virtual bool take(Player& p, Tile& r) = 0;
};

#endif
