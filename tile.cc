// Tile -- A tile in S.Y.T.Y.C.D...O.M.G? (implementation)
//

#include "item.h"
#include "tile.h"

Tile::Tile() {
    _contents = 0;
    _passable = false;
    _terrain = EMPTY;
}

Item* Tile::contents() {
    return _contents;
}

void Tile::setContents(Item* item) {
    if (item == 0) {
        delete _contents;
        _contents = 0;
    }
    else
        _contents = item;
}

void Tile::passable(bool p) {
    _passable = p;
}

bool Tile::isPassable() {
    return _passable;
}

TERRAIN Tile::terrain() {
    return _terrain;
}

void Tile::setTerrain(TERRAIN t) {
    _terrain = t;
}
