// Room -- A room in S.Y.T.Y.C.D...O.M.G? (implementation)
//

#include <cstdlib>
using namespace std;
#include "room.h"
#include "tile.h"
#include "world.h"

extern World* w;

Room::Room(int y, int x) {
    // randomly determine the size of the room.
    _y1 = y - (w->sectorHeight() / 4) - (rand() % (w->sectorHeight() / 4));
    if (_y1 < 1)
        _y1 = 1;
    _x1 = x - (w->sectorWidth() / 4) - (rand() % (w->sectorWidth() / 4));
    if (_x1 < 1)
        _x1 = 1;
    _y2 = y + (w->sectorHeight()) / 4 + (rand() % (w->sectorHeight() / 4));
    if (_y2 > (w->height() - 2))
        _y2 = w->height() - 2;
    _x2 = x + (w->sectorWidth() / 4) + (rand() % (w->sectorWidth() / 4));
    if (_x2 > (w->width() - 2))
        _x2 = w->width() - 2;

    int halfheight = _y1 + ((_y2 - _y1) / 2);
    int halfwidth = _x1 + ((_x2 - _x1) / 2);

    // Create the room by setting internal tiles to passable.  The first and
    // last rows and columns are set to walls so that there is allways a space
    // between rooms in other cells.  l is a random factor which gives a
    // chance of this being an L-shaped rather than rectangular room.
    // Doors are not working at the moment.
    int l = rand() % 6;
    for (int i = _y1; i < _y2; i++) {
        for (int j = _x1; j < _x2; j++) {
            if (j == _x1 || j == _x2 - 1) {
                if (w->tileAt(i,j).isPassable()) {
//                    w->tileAt(i,j).setTerrain(DOOR);
                    w->tileAt(i,j).setTerrain(FLOOR);
                    w->tileAt(i,j).setContents(w->makeItem());
                    w->tileAt(i,j).passable(true);
                }
                else {
                    w->tileAt(i,j).setTerrain(WALL);
                    w->tileAt(i,j).passable(false);
                    w->tileAt(i,j).setContents(0);
                }
            }
            else if (i == _y1 || i == _y2 - 1) {
                if (w->tileAt(i,j).isPassable()) {
//                    w->tileAt(i,j).setTerrain(DOOR);
                    w->tileAt(i,j).setTerrain(FLOOR);
                    w->tileAt(i,j).setContents(w->makeItem());
                    w->tileAt(i,j).passable(true);
                }
                else {
                    w->tileAt(i,j).setTerrain(WALL);
                    w->tileAt(i,j).passable(false);
                    w->tileAt(i,j).setContents(0);
                }
            }
            else {
                if ((l == 0 && i < halfheight && j <  halfwidth) ||
                (l == 1 && i < halfheight && j >= halfwidth) ||
                (l == 2 && i >= halfheight && j < halfwidth) ||
                (l == 3 && i >= halfheight && j >= halfwidth)) {
                    if (!w->tileAt(i,j).isPassable()) {
                        w->tileAt(i,j).passable(false);
                        w->tileAt(i,j).setTerrain(WALL);
                        w->tileAt(i,j).setContents(0);
                    }
                }
                else {
                    w->tileAt(i,j).passable(true);
                    w->tileAt(i,j).setTerrain(FLOOR);
                    w->tileAt(i,j).setContents(w->makeItem());
                }
            }
        }
    }
}

