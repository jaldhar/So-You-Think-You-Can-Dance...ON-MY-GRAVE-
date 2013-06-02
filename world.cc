// World -- The S.Y.T.Y.C.D...O.M.G? dungeon map and objects on it
// (implementation)
//

#include <algorithm>
#include <array>
#include <cstdlib>
#include <map>
#include <stdexcept>
#include <utility>
using namespace std;

#include "armor.h"
#include "door.h"
#include "monster.h"
#include "potion.h"
#include "room.h"
#include "shield.h"
#include "treasure.h"
#include "weapon.h"
#include "world.h"

typedef unique_ptr<Item>    ITEMPTR;
typedef unique_ptr<Room>    ROOMPTR;
typedef unique_ptr<Tile>    TILEPTR;

static const int MAP_HEIGHT       = 60;
static const int MAP_WIDTH        = 80;
static const int MAP_SECTORHEIGHT = 20;
static const int MAP_SECTORWIDTH  = 20;

struct World::WorldImpl {
    WorldImpl()=default;
    ~WorldImpl()=default;
    void addCorridors();
    void makeHorizontalCorridor(int row, int col1, int col2);
    void makeLCorridor(int ax, int ay, int bx, int by);
    void makeTwistyCorridor(int ax, int ay, int bx, int by);
    void makeVerticalCorridor(int row1, int row2, int col);
    void addDoors();
    void addItem();
    void placeItem(Item* item);
    void placePlayer();
    void addRooms(int sectorRows, int sectorCols);
    void addWalls();

    array<array<TILEPTR, MAP_WIDTH>,MAP_HEIGHT> _map;
    vector<ROOMPTR>                             _rooms;
    int                                         _playerRow;
    int                                         _playerCol;
    map<pair<int, int>, ITEMPTR>                _items;
    array<int, 6>                               _potions;
} World::_impl;


World::World()=default;

World::~World()=default;

void World::create() {
    // Begin by filling in the entire grid.
    for (auto & row : _impl._map) {
        for (auto & col : row) {
            col = TILEPTR(new Tile());
            col->setVisible(true);
        }
    }

    // Divide the grid into sectors.
    int sectorRows = MAP_HEIGHT / MAP_SECTORHEIGHT;
    int sectorCols = MAP_WIDTH / MAP_SECTORWIDTH;

    // Add a room in each sector.
    _impl.addRooms(sectorRows, sectorCols);

    // Add corridors
    _impl.addCorridors();

    // Now add the rooms. This goes after addCorridors() to prevent collisions.
    for(auto & r : _impl._rooms) {
        r->fill();
    }

    // Add walls.
    _impl.addWalls();

    // Add Doors
    _impl.addDoors();

    _impl._potions = {{ 1, 3, 1, 1, 6, 1 }};

    for (int i = 0; i < 100; i++) {
       _impl.addItem();
    }

    _impl.placePlayer();
}

int World::height() const {
    return MAP_HEIGHT;
}

int World::width() const {
    return MAP_WIDTH;
}

int World::playerRow() const {
    return _impl._playerRow;
}

void World::setPlayerRow(int row) {
    _impl._playerRow = row;
}

int World::playerCol() const {
    return _impl._playerCol;
}

void World::setPlayerCol(int col) {
    _impl._playerCol = col;
}

void  World::foreach_item(function<void(int, int, unique_ptr<Item>&)> callback) {
    for(auto & i : _impl._items) {

        callback(i.first.first, i.first.second, i.second);
    }
}

Item* World::itemAt(int row, int col) const {
    auto item = _impl._items.find(make_pair(row, col));
    if (item == _impl._items.end()) {
        return nullptr;
    }

    return item->second.get();
}

Item* World::removeItem(int row, int col) {
    auto item = _impl._items.find(make_pair(row, col));

    if (item == _impl._items.end()) {
        return nullptr;
    }

    Item* temp = item->second.release();
    _impl._items.erase(item);

    return temp;
}

Tile* World::tileAt(int row, int col) const {
    return _impl._map[row][col].get();
}

void World::WorldImpl::addCorridors() {
    // Create corridors by shuffling the list of rooms and going backwards
    // through it joining the center point of each room to that of the room
    // before it. Btw:  We can't use a ranged loop in reverse.

    random_shuffle(begin(_rooms), end(_rooms));

    for (auto rit = _rooms.rbegin(); rit < _rooms.rend() - 1; ++rit) {
        auto & r1 = *rit;
        auto & r2 = *(rit + 1); // previous room in list.
        makeLCorridor(r1->centerY(), r1->centerX(), r2->centerY(), r2->centerX());
    }

    // Join the last and the first rooms to complete the loop.
    auto & r1 = *(begin(_rooms));
    auto & r2 = *(end(_rooms) - 1);
    makeLCorridor(r1->centerY(), r1->centerX(), r2->centerY(), r2->centerX());
}

void World::WorldImpl::makeHorizontalCorridor(int row, int col1, int col2) {
    for (int i = col1; i <= col2; i++) {
        auto & t = _map[row][i];
        t->setPassable(true);
        t->setTerrain(TERRAIN::CORRIDOR);
    }
}

void World::WorldImpl::makeLCorridor(int ay, int ax, int by, int bx) {

    // decide direction in which we will join segments
    bool flip = rand() % 2 ? true : false;

    if (ax < bx) {                  // a to west of b
        if (flip) {
            makeHorizontalCorridor(by, ax, bx);
        } else {
            makeHorizontalCorridor(ay, ax, bx);
        }
    } else {                            // a to east of b
        if (flip) {
            makeHorizontalCorridor(by, bx, ax);
        } else {
            makeHorizontalCorridor(ay, bx, ax);
        }
    }

    if (ay < by) {                      // a to north of b
        if (flip) {
            makeVerticalCorridor(ay, by, ax);
        } else {
            makeVerticalCorridor(ay, by, bx);
        }
    } else {                            // a to south of b
        if (flip) {
            makeVerticalCorridor(by, ay, ax);
        } else {
            makeVerticalCorridor(by, ay, bx);
        }
    }
}

void World::WorldImpl::makeTwistyCorridor(int ay, int ax, int by, int bx) {

    // decide direction in which we will join segments
    bool flip = rand() % 2 ? true : false;

    int cx, cy, diffx, diffy;

    diffx = abs(ax - bx);
    diffy = abs(ay - by);
    // prevent divide by 0 errors when diffx, diffy are too small.
    if (diffx < 4) {
        diffx = 4;
    }
    if (diffy < 4) {
        diffy = 4;
    }

    if (ax < bx) {
        cx = ax + rand() % (diffx / 2) + (diffx / 4);
    } else {
        cx = bx + rand() % (diffx / 2) + (diffx / 4);
    }

    if (ay < by) {
        cy = ay + rand() % (diffy / 2) + (diffy / 4);
    } else {
        cy = by + rand() % (diffy / 2) + (diffy / 4);
    }

    if (ax < bx) {                      // a to west of b
        if (flip) {
            makeHorizontalCorridor(cy, ax, bx);
        } else {
            makeHorizontalCorridor(ay, ax, cx);
            makeHorizontalCorridor(by, cx, bx);
        }
    } else {                            // a to east of b
        if (flip) {
            makeHorizontalCorridor(cy, bx, ax);
        } else {
            makeHorizontalCorridor(ay, cx, ax);
            makeHorizontalCorridor(by, bx, cx);
        }
    }

    if (ay < by) {                      // a to north of b
        if (flip) {
            makeVerticalCorridor(ay, cy, ax);
            makeVerticalCorridor(cy, by, bx);
        } else {
            makeVerticalCorridor(ay, by, cx);
        }
    } else {                            // a to south of b
        if (flip) {
            makeVerticalCorridor(cy, ay, ax);
            makeVerticalCorridor(by, cy, bx);
        } else {
            makeVerticalCorridor(by, ay, cx);
        }
    }
}

void World::WorldImpl::makeVerticalCorridor(int row1, int row2, int col) {
    for (int i = row1; i <= row2; i++) {
        auto & t = _map[i][col];
        t->setPassable(true);
        t->setTerrain(TERRAIN::CORRIDOR);
    }
}

void World::WorldImpl::addDoors() {
    for(auto & r : _impl._rooms) {

        // Vertical doors.
        for (int row = r->top() - 1; row < r->top() + r->height() + 1; row++) {
            for (int col = r->left() - 1; col <= r->left() + r->width() + 1; col += r->width() + 1) {
                if(_map[row - 1][col]->terrain() != TERRAIN::CORRIDOR &&
                    _map[row + 1][col]->terrain()!= TERRAIN::CORRIDOR &&
                    _map[row][col]->terrain() == TERRAIN::CORRIDOR) {
                    Door *d = new Door();
                    _items[make_pair(row, col)] = ITEMPTR(d);
                }
            }
        }

        // Horizontal doors.
        for (int col = r->left() - 1; col < r->left() + r->width() + 1; col++) {
            for (int row = r->top() - 1; row <= r->top() + r->height() + 1; row += r->height() + 1) {
               if(_map[row][col - 1]->terrain() != TERRAIN::CORRIDOR &&
                    _map[row][col + 1]->terrain() != TERRAIN::CORRIDOR &&
                    _map[row][col]->terrain() == TERRAIN::CORRIDOR) {
                    Door *d = new Door();
                    d->setHorizontal(true);
                    _items[make_pair(row, col)] = ITEMPTR(d);
                }
            }
        }
    }
}

void World::WorldImpl::addItem() {
    int r = rand() % 100;

    if (r < 50) {
    }

    else if (r < 60) {
        placeItem(new Treasure());
    }

    else if (r < 75) {
        int number = _potions[rand() % 6];
        placeItem(new Potion(number));
    }

    else if (r < 80) {
        switch(rand() % 6) {
            case 0:
              placeItem(new Shield("a", "buckler", ITEMTYPE::BUCKLER, 1, 0));
              break;
            case 1:
              placeItem(new Shield("a", "shield", ITEMTYPE::SHIELD, 2, 0));
              break;
            case 2:
              placeItem(new Weapon("a", "sword", ITEMTYPE::SWORD, 0, 1));
              break;
            case 3:
              placeItem(new Weapon("an",   "axe", ITEMTYPE::AXE, 0, 2));
              break;
            case 4:
              placeItem(new Armor("some", "chain mail", ITEMTYPE::CHAINMAIL, 3, 0));
              break;
            case 5:
              placeItem(new Armor("some", "plate mail", ITEMTYPE::PLATEMAIL, 5, 0));
              break;
        };
    }

    else if (r < 84) {
        placeItem(new Monster("a", "zombie", ITEMTYPE::ZOMBIE, 1, 1, 0));
    }

    else if (r < 88) {
        placeItem(new Monster("a", "skeleton", ITEMTYPE::SKELETON, 1, 2, 0));
    }

    else if (r < 91) {
        placeItem(new Monster("a", "ghoul", ITEMTYPE::GHOUL, 2, 2, 0));
    }
    else if (r < 94) {
        placeItem(new Monster("a", "wight", ITEMTYPE::WIGHT, 2, 3, 0));
    }

    else if (r < 96) {
        placeItem(new Monster("a", "vampire", ITEMTYPE::VAMPIRE, 3, 3, 0));
    }

    else if (r < 98) {
        placeItem(new Monster("an", "imp", ITEMTYPE::IMP, 3, 4, 3));
    }

    else if (r < 99) {
        placeItem(new Monster("a", "demon", ITEMTYPE::DEMON, 5, 5, 5));
    }

    else {
        placeItem(new Monster("a", "fiend", ITEMTYPE::FIEND, 10, 5, 5));
    }
//    _monsters["Pharaz"]   = Monster('P', "Pharaz",   "the dread lich", 10,10,9);
}

void World::WorldImpl::placeItem(Item* item) {
    int row, col;

    do {
        row = rand() % MAP_HEIGHT;
        col = rand() % MAP_WIDTH;
    } while (!_map[row][col]->passable() ||
        _items.find(make_pair(row, col)) != _items.end());

    _items[make_pair(row, col)] = ITEMPTR(item);
}

// Position player on a passable tile which doesn't already have an item or
// monster on it.
void World::WorldImpl::placePlayer() {
    int row, col;

    do {
        row = rand() % MAP_HEIGHT;
        col = rand() % MAP_WIDTH;
    } while (!_map[row][col]->passable() ||
        _items.find(make_pair(row, col)) != _items.end());

    _playerRow = row;
    _playerCol = col;
}

void World::WorldImpl::addRooms(int sectorRows, int sectorCols) {
    int emptyrooms = 0;

    for (int row = 0; row < sectorRows; row++) {
        for (int col = 0; col < sectorCols; col++) {

            // 25% chance of no room.  Can only happen maximum 3 times per
            // level.
            if (rand() % 4 == 0 && emptyrooms < 3) {
                emptyrooms++;
                continue;
            }

            int yoffset = row * MAP_SECTORHEIGHT;
            int xoffset = col * MAP_SECTORWIDTH;
            _rooms.push_back(ROOMPTR(new Room(yoffset, xoffset,
                MAP_SECTORHEIGHT, MAP_SECTORWIDTH)));
        }
    }
}

void World::WorldImpl::addWalls() {
   //First pass puts a center wall around everything
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {

            auto & t = _map[row][col];

            if (t->terrain() != TERRAIN::EMPTY) {
                continue;
            }

            for (int x = row - 1; x < row + 2; x++) {

                if (x < 0 || x >= MAP_HEIGHT) {
                    continue;
                }

                for (int y = col - 1; y < col + 2; y++) {

                    if (y < 0 || y >= MAP_WIDTH) {
                        continue;
                    }

                    if (x == row && y == col) {
                        continue;
                    }

                    TERRAIN c = _map[x][y]->terrain();
                    if (c == TERRAIN::FLOOR || c == TERRAIN::CORRIDOR) {
                        t->setTerrain(TERRAIN::C_WALL);
                        t->setPassable(false);
                        goto end;
                    }
                }
            }

            end: ;
        }
    }

  // Second pass makes specific wall types as needed.
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {

            auto & t = _map[row][col];

            if (t->terrain() != TERRAIN::C_WALL) {
                continue;
            }

            int count = 7, edges = 0; // represent the edges as a binary number
            // TODO: use bitset here maybe.
            for (int y = row - 1; y < row + 2; y++) {

                if (y < 0 || y > MAP_HEIGHT - 1) {
                    count -= 3;
                    continue;
                }

                for (int x = col - 1; x < col + 2; x++) {
                    if (x < 0 || x > MAP_WIDTH - 1) {
                        count--;
                        continue;
                    }

                    if (y == row && x == col) {
                        continue;
                    }

                    if (_map[y][x]->isBlock()) {
                        edges += pow(2, count);
                    }

                    count--;
                }
            }

            if ((edges & 90) == 90) {              // = 01011010
                t->setTerrain(TERRAIN::C_WALL);
            } else if ((edges & 88) == 88) {       // = 01011000
                t->setTerrain(TERRAIN::BT_WALL);
            } else if ((edges & 82) == 82) {       // = 01010010
                t->setTerrain(TERRAIN::RT_WALL);
            } else if ((edges & 74) == 74) {       // = 01001010
                t->setTerrain(TERRAIN::LT_WALL);
            } else if ((edges & 26) == 26) {       // = 00011010
                t->setTerrain(TERRAIN::TT_WALL);
            } else if ((edges & 80) == 80) {       // = 01010000
                t->setTerrain(TERRAIN::LR_WALL);
            } else if ((edges & 72) == 72) {       // = 01001000
                t->setTerrain(TERRAIN::LL_WALL);
            } else if ((edges & 18) == 18) {       // = 00010010
                t->setTerrain(TERRAIN::UR_WALL);
            } else if ((edges & 10) == 10) {       // = 00001010
               t->setTerrain(TERRAIN::UL_WALL);
            } else if ((edges & 66) == 66 || (edges & 64) == 64 ||
                (edges & 2) == 2) {             // = 01000010,01000000,00000010
                t->setTerrain(TERRAIN::V_WALL);
            } else if ((edges & 24) == 24 || (edges & 16) == 16 ||
                (edges & 8) == 8) {             // = 00011000,00010000,00001000
                t->setTerrain(TERRAIN::H_WALL);
            } else if ((edges & 0) == 0) {         // = 00000000
                t->setTerrain(TERRAIN::C_WALL);
            } else {
                fprintf(stderr, "%d\n", edges);    // For debugging.
            }
        }
    }
}
