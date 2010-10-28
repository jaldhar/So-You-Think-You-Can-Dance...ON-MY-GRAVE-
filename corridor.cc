// Room -- A room in S.Y.T.Y.C.D...O.M.G? (implementation)
//

#include <cstdlib>
using namespace std;
#include "corridor.h"
#include "tile.h"
#include "world.h"

extern World* w;

Corridor::Corridor(int x1, int y1, int x2, int y2) {
    _y1 = y1;
    _x1 = x1;
    _y2 = y2;
    _x2 = x2;
    int i = _x1, j = _y1;

    w->tileAt(i,j).passable(true);
    w->tileAt(i,j).setTerrain(FLOOR);
    w->tileAt(i,j).setContents(w->makeItem());
    if (i < _x2) {

        wallfill_north(i, j);

        while (i != _x2) {
            w->tileAt(++i,j).passable(true);
            w->tileAt(i,j).setTerrain(FLOOR);
            w->tileAt(i,j).setContents(w->makeItem());
            if ((j - 1) > 0 && !w->tileAt(i,j - 1).isPassable()) {
                w->tileAt(i, j - 1).setTerrain(WALL);
                w->tileAt(i, j - 1).setContents(0);
            }
            if ((j + 1) < (w->width() - 1) && !w->tileAt(i,j + 1).isPassable()){
                w->tileAt(i, j + 1).setTerrain(WALL);
                w->tileAt(i, j + 1).setContents(0);
            }
        }

        wallfill_south(i, j);
    }
    if (i > x2) {

        wallfill_south(i, j);

        while (i != _x2) {
            w->tileAt(--i,j).passable(true);
            w->tileAt(i,j).setTerrain(FLOOR);
            w->tileAt(i,j).setContents(w->makeItem());
            if ((j - 1) > 0 && !w->tileAt(i,j - 1).isPassable()) {
                w->tileAt(i, j - 1).setTerrain(WALL);
                w->tileAt(i, j - 1).setContents(0);
            }
            if ((j + 1) < (w->width() - 1) && !w->tileAt(i,j + 1).isPassable()){
                w->tileAt(i, j + 1).setTerrain(WALL);
                w->tileAt(i, j + 1).setContents(0);
            }
        }

        wallfill_north(i, j);
    }
    if (j < y2) {

        wallfill_west(i, j);

        while (j != _y2) {
            w->tileAt(i,++j).passable(true);
            w->tileAt(i,j).setTerrain(FLOOR);
            w->tileAt(i,j).setContents(w->makeItem());
            if ((i - 1) > 0 && !w->tileAt(i - 1,j).isPassable()) {
                w->tileAt(i - 1, j).setTerrain(WALL);
                w->tileAt(i - 1, j).setContents(0);
            }
            if ((i + 1) < (w->height() - 1) && !w->tileAt(i + 1,j).isPassable()){
                w->tileAt(i + 1, j).setTerrain(WALL);
                w->tileAt(i + 1, j).setContents(0);
            }
        }

        wallfill_east(i, j);
    }
    if (j > y2) {

        wallfill_east(i, j);

        while (j != _y2) {
            w->tileAt(i,--j).passable(true);
            w->tileAt(i,j).setTerrain(FLOOR);
            w->tileAt(i,j).setContents(w->makeItem());
            if ((i - 1) > 0 && !w->tileAt(i - 1,j).isPassable()) {
                w->tileAt(i - 1, j).setTerrain(WALL);
                w->tileAt(i - 1, j).setContents(0);
            }
            if ((i + 1) < (w->height() - 1) && !w->tileAt(i + 1,j).isPassable()) {
                w->tileAt(i + 1, j).setTerrain(WALL);
                w->tileAt(i + 1, j).setContents(0);
            }
        }

        wallfill_west(i, j);
    }
}

void Corridor::wallfill_north(int y, int x) {
    if (y > 0 && !w->tileAt(y - 1, x).isPassable()) {
        w->tileAt(y - 1, x).setTerrain(WALL);
        if (x > 0 && !w->tileAt(y - 1, x - 1).isPassable()) {
            w->tileAt(y - 1, x - 1).setTerrain(WALL);
            w->tileAt(y - 1, x - 1).setContents(0);
        }
        if (x < (w->width() - 1) && !w->tileAt(y - 1, x + 1).isPassable()) {
            w->tileAt(y - 1, x + 1).setTerrain(WALL);
            w->tileAt(y - 1, x + 1).setContents(0);
        }
    }
}

void Corridor::wallfill_south(int y, int x) {
    if (y < (w->height() - 1) && !w->tileAt(y + 1, x).isPassable()) {
        w->tileAt(y + 1, x).setTerrain(WALL);
        if (x > 0 && !w->tileAt(y + 1, x - 1).isPassable()) {
            w->tileAt(y + 1, x - 1).setTerrain(WALL);
            w->tileAt(y + 1, x - 1).setContents(0);
        }
        if (x < (w->width() - 1) && !w->tileAt(y + 1, x + 1).isPassable()) {
            w->tileAt(y + 1, x + 1).setTerrain(WALL);
            w->tileAt(y + 1, x + 1).setContents(0);
        }
    }
}

void Corridor::wallfill_west(int y, int x) {
    if (x > 0 && !w->tileAt(y, x - 1).isPassable()) {
        w->tileAt(y, x - 1).setTerrain(WALL);
        if (y > 0 && !w->tileAt(y - 1, x - 1).isPassable()) {
            w->tileAt(y - 1, x - 1).setTerrain(WALL);
            w->tileAt(y - 1, x - 1).setContents(0);
        }
        if (y < (w->height() - 1) && !w->tileAt(y + 1, x - 1).isPassable()) {
            w->tileAt(y + 1, x - 1).setTerrain(WALL);
            w->tileAt(y + 1, x - 1).setContents(0);
        }
    }
}

void Corridor::wallfill_east(int y, int x) {
    if (x < (w->width() - 1) && !w->tileAt(y, x + 1).isPassable()) {
        w->tileAt(y, x + 1).setTerrain(WALL);
        if (y > 0 && !w->tileAt(y - 1, x + 1).isPassable()) {
            w->tileAt(y - 1, x + 1).setTerrain(WALL);
            w->tileAt(y - 1, x + 1).setContents(0);
        }
        if (y < (w->height() - 1) && !w->tileAt(y + 1, x + 1).isPassable()) {
            w->tileAt(y + 1, x + 1).setTerrain(WALL);
            w->tileAt(y + 1, x + 1).setContents(0);
        }
    }
}
