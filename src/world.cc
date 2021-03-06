// World -- The S.Y.T.Y.C.D...O.M.G? dungeon map and objects on it
// (implementation)
//

#include <algorithm>
#include <array>
#include <bitset>
#include <cstdlib>
#include <map>
#include <stdexcept>
#include <utility>

#include "armor.h"
#include "door.h"
#include "monster.h"
#include "potion.h"
#include "room.h"
#include "shield.h"
#include "treasure.h"
#include "weapon.h"
#include "world.h"

using RoomPtr = std::unique_ptr<Room>;
using TilePtr = std::unique_ptr<Tile>;

constexpr int MAP_HEIGHT       = 60;
constexpr int MAP_WIDTH        = 80;
constexpr int MAP_SECTORHEIGHT = 20;
constexpr int MAP_SECTORWIDTH  = 20;

struct World::WorldImpl {
    WorldImpl();
    ~WorldImpl()=default;
    void addCorridors();
    void makeHorizontalCorridor(int row, int col1, int col2);
    void makeLCorridor(int ax, int ay, int bx, int by);
    void makeTwistyCorridor(int ax, int ay, int bx, int by);
    void makeVerticalCorridor(int row1, int row2, int col);
    void addDoors();
    void addItem(int top, int left, int height, int width);
    Item* createItem();
    void placePlayer();
    void addRooms(int sectorRows, int sectorCols);
    void fillRoom(RoomPtr& r);
    void addWalls();
    void specializeWalls();

    std::array<std::array<TilePtr, MAP_WIDTH>,MAP_HEIGHT>   map_;
    std::vector<RoomPtr>                                    rooms_;
    int                                                     playerRow_;
    int                                                     playerCol_;
    std::map<std::pair<int, int>, ITEMPTR>                  items_;
} World::impl_;


World::World()=default;

World::~World()=default;

void World::create() {
    // Begin by filling in the entire grid.
    for (auto & row : impl_.map_) {
        for (auto & col : row) {
            col = TilePtr(new Tile());
            col->setVisible(true);
        }
    }

    // Divide the grid into sectors.
    int sectorRows = MAP_HEIGHT / MAP_SECTORHEIGHT;
    int sectorCols = MAP_WIDTH / MAP_SECTORWIDTH;

    // Add a room in each sector.
    impl_.addRooms(sectorRows, sectorCols);

    // Add corridors
    impl_.addCorridors();

    // Now fill the rooms. This goes after addCorridors() to prevent collisions.
    for(auto & r : impl_.rooms_) {
        impl_.fillRoom(r);
    }

    // Add basic walls.
    impl_.addWalls();

    // Add Doors
    impl_.addDoors();

    // Add better looking wall types.
    impl_.specializeWalls();

    impl_.placePlayer();
}

int World::height() const {
    return MAP_HEIGHT;
}

int World::width() const {
    return MAP_WIDTH;
}

int World::playerRow() const {
    return impl_.playerRow_;
}

void World::setPlayerRow(int row) {
    impl_.playerRow_ = row;
}

int World::playerCol() const {
    return impl_.playerCol_;
}

void World::setPlayerCol(int col) {
    impl_.playerCol_ = col;
}

void  World::foreach_item(int top, int left, int height, int width,
    std::function<void(int, int, ITEMPTR&)> callback) {
    for(auto & i : impl_.items_) {
        int row = i.first.first;
        int col = i.first.second;
        if (row < top || row > top + height - 1 || col < left ||
        col > left + width - 1) {
            continue;
        }
        callback(row, col, i.second);
    }
}

Item* World::itemAt(int row, int col) const {
    auto item = impl_.items_.find(std::make_pair(row, col));
    if (item == impl_.items_.end()) {
        return nullptr;
    }

    return item->second.get();
}

bool World::removeItem(int row, int col, bool destroy) {
    auto item = impl_.items_.find(std::make_pair(row, col));

    if (item == impl_.items_.end()) {
        return false;
    }

    if (destroy) {
        delete item->second.release();
    } else {
        item->second.release();
    }
    impl_.items_.erase(item);

    return true;
}

Tile* World::tileAt(int row, int col) const {
    return impl_.map_[row][col].get();
}

World::WorldImpl::WorldImpl() : map_{}, rooms_{}, playerRow_{0}, playerCol_{0}, items_{} {
}

void World::WorldImpl::addCorridors() {
    // Create corridors by shuffling the list of rooms and going backwards
    // through it joining the center point of each room to that of the room
    // before it. Btw:  We can't use a ranged loop in reverse.

    std::random_shuffle(rooms_.begin(), rooms_.end());

    for (auto rit = rooms_.rbegin(); rit < rooms_.rend() - 1; ++rit) {
        auto & r1 = *rit;
        auto & r2 = *(rit + 1); // previous room in list.
        makeLCorridor(r1->centerY(), r1->centerX(), r2->centerY(), r2->centerX());
    }

    // Join the last and the first rooms to complete the loop.
    auto & r1 = *(rooms_.begin());
    auto & r2 = *(rooms_.end() - 1);
    makeLCorridor(r1->centerY(), r1->centerX(), r2->centerY(), r2->centerX());
}

void World::WorldImpl::makeHorizontalCorridor(int row, int col1, int col2) {
    for (int i = col1; i <= col2; i++) {
        auto & t = map_[row][i];
        t->setPassable(true);
        t->setTerrain(TERRAIN::CORRIDOR);
    }
    if (std::rand() % 10) { // 10% chance of an item in the corridor
       impl_.addItem(row, col1, 1, col2 - col1 + 1);
    }
}

void World::WorldImpl::makeLCorridor(int ay, int ax, int by, int bx) {

    // decide direction in which we will join segments
    bool flip = std::rand() % 2 ? true : false;

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
    bool flip = std::rand() % 2 ? true : false;

    int cx, cy, diffx, diffy;

    diffx = std::abs(ax - bx);
    diffy = std::abs(ay - by);
    // prevent divide by 0 errors when diffx, diffy are too small.
    if (diffx < 4) {
        diffx = 4;
    }
    if (diffy < 4) {
        diffy = 4;
    }

    if (ax < bx) {
        cx = ax + std::rand() % (diffx / 2) + (diffx / 4);
    } else {
        cx = bx + std::rand() % (diffx / 2) + (diffx / 4);
    }

    if (ay < by) {
        cy = ay + std::rand() % (diffy / 2) + (diffy / 4);
    } else {
        cy = by + std::rand() % (diffy / 2) + (diffy / 4);
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
        auto & t = map_[i][col];
        t->setPassable(true);
        t->setTerrain(TERRAIN::CORRIDOR);
    }
    if (std::rand() % 10) { // 10% chance of an item in the corridor
       impl_.addItem(row1, col, row2 - row1 + 1, 1);
    }
}

void World::WorldImpl::addDoors() {
    for(auto & r : impl_.rooms_) {

        // Vertical doors.
        for (int row = r->top() - 1; row < r->top() + r->height() + 1; row++) {
            for (int col = r->left() - 1; col <= r->left() + r->width() + 1; col += r->width() + 1) {
                if(map_[row - 1][col]->terrain() != TERRAIN::CORRIDOR &&
                    map_[row + 1][col]->terrain()!= TERRAIN::CORRIDOR &&
                    map_[row][col]->terrain() == TERRAIN::CORRIDOR) {
                    Door *d = new Door();
                    items_[std::make_pair(row, col)] = ITEMPTR(d);
                }
            }
        }

        // Horizontal doors.
        for (int col = r->left() - 1; col < r->left() + r->width() + 1; col++) {
            for (int row = r->top() - 1; row <= r->top() + r->height() + 1; row += r->height() + 1) {
               if(map_[row][col - 1]->terrain() != TERRAIN::CORRIDOR &&
                    map_[row][col + 1]->terrain() != TERRAIN::CORRIDOR &&
                    map_[row][col]->terrain() == TERRAIN::CORRIDOR) {
                    Door *d = new Door();
                    d->setHorizontal(true);
                    items_[std::make_pair(row, col)] = ITEMPTR(d);
                }
            }
        }
    }
}

void World::WorldImpl::addItem(int top, int left, int height,
    int width) {
    int row, col;

    Item* item = createItem();
    if (item == nullptr) {
        return;
    }

    do {
        row = top;
        if (height > 1) {
            row += std::rand() % height;
        }
        col = left;
        if (width > 1) {
            col += std::rand() % width;
        }
    } while (!map_[row][col]->passable() ||
        items_.find(std::make_pair(row, col)) != items_.end());

    items_[std::make_pair(row, col)] = ITEMPTR(item);
}

Item* World::WorldImpl::createItem() {
    int r = std::rand() % 100;

    if (r < 50) {
    }

    else if (r < 60) {
        return new Treasure();
    }

    else if (r < 75) {
        std::array<int, 6> potions = {{ 1, 3, 1, 1, 6, 1 }};
        int number = potions[std::rand() % 6];

        return new Potion(number);
    }

    else if (r < 80) {
        switch(std::rand() % 6) {
            case 0:
              return new Shield("a", "buckler", ITEMTYPE::BUCKLER, 1, 0);
              break;
            case 1:
              return new Shield("a", "shield", ITEMTYPE::SHIELD, 2, 0);
              break;
            case 2:
              return new Weapon("a", "sword", ITEMTYPE::SWORD, 0, 1);
              break;
            case 3:
              return new Weapon("an",   "axe", ITEMTYPE::AXE, 0, 2);
              break;
            case 4:
              return new Armor("some", "chain mail", ITEMTYPE::CHAINMAIL, 3, 0);
              break;
            case 5:
              return new Armor("some", "plate mail", ITEMTYPE::PLATEMAIL, 5, 0);
              break;
        };
    }

    else if (r < 84) {
        return new Monster("a", "zombie", ITEMTYPE::ZOMBIE, 1, 1, 0);
    }

    else if (r < 88) {
        return new Monster("a", "skeleton", ITEMTYPE::SKELETON, 1, 2, 0);
    }

    else if (r < 91) {
        return new Monster("a", "ghoul", ITEMTYPE::GHOUL, 2, 2, 0);
    }
    else if (r < 94) {
        return new Monster("a", "wight", ITEMTYPE::WIGHT, 2, 3, 0);
    }

    else if (r < 96) {
        return new Monster("a", "vampire", ITEMTYPE::VAMPIRE, 3, 3, 0);
    }

    else if (r < 98) {
        return new Monster("an", "imp", ITEMTYPE::IMP, 3, 4, 3);
    }

    else if (r < 99) {
        return new Monster("a", "demon", ITEMTYPE::DEMON, 5, 5, 5);
    }

    else {
        return new Monster("a", "fiend", ITEMTYPE::FIEND, 10, 5, 5);
    }
//    _monsters["Pharaz"]   = Monster('P', "Pharaz",   "the dread lich", 10,10,9);
    return nullptr;
}

// Position player on a passable tile which doesn't already have an item or
// monster on it.
void World::WorldImpl::placePlayer() {
    int row, col;

    do {
        row = std::rand() % MAP_HEIGHT;
        col = std::rand() % MAP_WIDTH;
    } while (!map_[row][col]->passable() ||
        items_.find(std::make_pair(row, col)) != items_.end());

    playerRow_ = row;
    playerCol_ = col;
}

void World::WorldImpl::addRooms(int sectorRows, int sectorCols) {
    int emptyrooms = 0;

    for (int row = 0; row < sectorRows; row++) {
        for (int col = 0; col < sectorCols; col++) {

            // 25% chance of no room.  Can only happen maximum 3 times per
            // level.
            if (std::rand() % 4 == 0 && emptyrooms < 3) {
                emptyrooms++;
                continue;
            }

            int yoffset = row * MAP_SECTORHEIGHT;
            int xoffset = col * MAP_SECTORWIDTH;
            rooms_.push_back(RoomPtr(new Room(yoffset, xoffset,
                MAP_SECTORHEIGHT, MAP_SECTORWIDTH)));
        }
    }
}

void World::WorldImpl::fillRoom(RoomPtr& r) {
    for (int row = r->top(); row < r->top() + r->height(); row++) {
        for (int col = r->left(); col < r->left() + r->width(); col++) {
            auto & t = map_[row][col];
            t->setPassable(true);
            t->setTerrain(TERRAIN::FLOOR);
        }
    }

    for (int i = 0; i < 10; i++) {
       impl_.addItem(r->top(), r->left(), r->height(), r->width());
    }


}

void World::WorldImpl::addWalls() {
    //First pass puts a center wall adjacent to any floor or corridor.
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {

            auto & t = map_[row][col];

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

                    TERRAIN c = map_[x][y]->terrain();
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
}

void World::WorldImpl::specializeWalls() {

    // Second pass makes specific wall types as needed.
    std::map<std::string, TERRAIN> walls = {
        {"01011010", TERRAIN::C_WALL},
        {"01011000", TERRAIN::BT_WALL},
        {"01010010", TERRAIN::RT_WALL},
        {"01010000", TERRAIN::LR_WALL},
        {"01001010", TERRAIN::LT_WALL},
        {"01001000", TERRAIN::LL_WALL},
        {"01000010", TERRAIN::V_WALL},
        {"01000000", TERRAIN::V_WALL},
        {"00011010", TERRAIN::TT_WALL},
        {"00011000", TERRAIN::H_WALL},
        {"00010000", TERRAIN::H_WALL},
        {"00010010", TERRAIN::UR_WALL},
        {"00001010", TERRAIN::UL_WALL},
        {"00001000", TERRAIN::H_WALL},
        {"00000010", TERRAIN::V_WALL},
        {"00000000", TERRAIN::C_WALL},
    };

    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {

            auto & t = map_[row][col];

            if (t->terrain() != TERRAIN::C_WALL) {
                continue;
            }

            int count = 7;
            std::bitset<8> edgeset; // represent the edges as a binary number
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

                    if (map_[y][x]->isBlock()) {
                        edgeset.set(count);
                    }

                    auto item = items_.find(std::make_pair(y, x));
                    if (item != items_.end() &&
                    dynamic_cast<Door*>(item->second.get())) {
                        edgeset.set(count);
                    }

                    count--;
                }
            }

            for (auto& wall: walls) {
                std::bitset<8> mask(wall.first);
                if ((edgeset & mask) == mask) {
                    t->setTerrain(wall.second);
                }
            }
        }
    }
}
