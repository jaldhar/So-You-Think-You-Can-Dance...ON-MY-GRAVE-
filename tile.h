// Tile -- A tile in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef TILE_H
#define TILE_H 1

#include <memory>
#include "terrain.h"

class Tile {
public:
    Tile();
    ~Tile();
    bool    passable() const;
    void    setPassable(bool p);
    TERRAIN terrain() const;
    void    setTerrain(TERRAIN terrain);
    bool    visible() const;
    void    setVisible(bool v);
    bool    isBlock();
    bool    isDoor();

private:
    struct TileImpl;
    std::unique_ptr<TileImpl> _impl;
};

#endif
