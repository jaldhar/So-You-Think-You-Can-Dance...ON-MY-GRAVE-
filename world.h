// World -- The S.Y.T.Y.C.D...O.M.G?  dungeon map and objects on it (interface)
//

#ifndef WORLD_H
#define WORLD_H 1

#include <functional>
#include "item.h"
#include "monster.h"
#include "tile.h"

class World {
public:
    World();
    ~World();
    void     create();
    int      height() const;
    int      width() const;
    int      playerRow() const;
    void     setPlayerRow(int row);
    int      playerCol() const;
    void     setPlayerCol(int col);
    void     foreach_item(std::function<void(int, int, std::unique_ptr<Item>&)> callback);
    Item*    itemAt(int row, int col) const;
    bool     removeItem(int row, int col, bool destroy = false);
    Tile*    tileAt(int row, int col) const;
private:
    struct WorldImpl;
    static WorldImpl _impl;
};

#endif
