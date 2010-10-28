// Tile -- A tile in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef TILE_H
#define TILE_H 1

#include "terrain.h"
class Item;

class Tile {
public:
    Tile();
    Item*   contents();
    void    setContents(Item* item);
    void    passable(bool p);
    bool    isPassable();
    TERRAIN terrain();
    void    setTerrain(TERRAIN t);
private:
    bool    _passable;
    Item*   _contents;
    TERRAIN _terrain;
};

#endif
