// World -- The S.Y.T.Y.C.D...O.M.G? dungeon map and objects on it
// (implementation)
//

#include <sstream>
#include <string>
using namespace std;

#include "corridor.h"
#include "item.h"
#include "monster.h"
#include "player.h"
#include "potion.h"
#include "room.h"
#include "tile.h"
#include "treasure.h"
#include "world.h"

World* World::_instance = 0;

World* World::instance(){
    if(_instance == 0)
        _instance = new World();

    return _instance;
}

World::World() {
    _items.push_back(new Shield("buckler",      "a",    0, 1));
    _items.push_back(new Shield("shield",       "a",    0, 2));
    _items.push_back(new Weapon("sword",        "a",    1, 0));
    _items.push_back(new Weapon("axe",          "an",   2, 0));
    _items.push_back(new Armor("chain mail",    "some", 0, 3));
    _items.push_back(new Armor("plate mail",    "some", 0, 5));

    _monsters["Bat"]      = Monster('b', "Bat",      "a",  1,  1,  0);
    _monsters["Rat"]      = Monster('r', "Rat",      "a",  1,  2,  0);
    _monsters["Skeleton"] = Monster('s', "Skeleton", "a",  2,  2,  0);
    _monsters["Ghoul"]    = Monster('g', "Ghoul",    "a",  2,  3,  0);
    _monsters["Wight"]    = Monster('w', "Wight",    "a",  3,  3,  0);
    _monsters["Vampire"]  = Monster('v', "Vampire",  "a",  3,  4,  3);
    _monsters["Demon"]    = Monster('d', "Demon",    "a",  5,  5,  5);
    _monsters["Fiend"]    = Monster('f', "Fiend",    "a",  10, 5,  5);
    _monsters["Pharaz"]   = Monster('P', "Pharaz",   "the dread lich", 10,10,9);

    _potions = { 1, 3, 1, 1, 6, 1 };

    _player = new Player;
}

World::~World(){
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            if(_map[row][col] != 0)
                delete _map[row][col];
        }
    }

    if (_player)
        delete _player;

    if(_instance != 0)
        delete _instance;
}

void World::create() {
    // Begin by filling in the entire grid.
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            _map[row][col] = new Tile();
        }
    }
    // Divide the grid into sectors.  Select a center point for each sector
    // and add it to a vector.
    int maxheight = MAP_HEIGHT / MAP_SECTORHEIGHT;
    int maxwidth = MAP_WIDTH / MAP_SECTORWIDTH;
    vector <pair<int,int>> centers;
    for (int row = 0; row < maxheight; row++) {
        for (int col = 0; col < maxwidth; col++) {
            int i = (row * MAP_SECTORHEIGHT) + (MAP_SECTORHEIGHT / 4) +
                (rand() % MAP_SECTORHEIGHT / 2);
            int j = (col * MAP_SECTORWIDTH) + (MAP_SECTORWIDTH / 4) +
                (rand() % MAP_SECTORWIDTH / 2);
            centers.push_back(make_pair(i,j));
        }
    }

    // Make corridors first.
    // W -> E
    for (int row = 0; row < maxheight; row++) {
        for (int col = 0; col < (maxwidth - 1); col++) {
            pair<int,int> cell = centers.at((row * maxwidth) + col);
            pair<int,int> nextcell = centers.at((row * maxwidth) + (col + 1));
            Corridor c(cell.first, cell.second, nextcell.first, nextcell.second);
        }
    }
    // N -> S
    for (int row = 0; row < (maxheight - 1); row++) {
        for (int col = 0; col < maxwidth; col++) {
            pair<int,int> cell = centers.at((row * maxwidth) + col);
            pair<int,int> nextcell = centers.at(((row + 1) * maxwidth) + col);
            Corridor c(cell.first,cell.second,nextcell.first,nextcell.second);
        }
    }
    // Then make rooms.
    for (int row = 0; row < maxheight; row++) {
        for (int col = 0; col < maxwidth; col++) {
            pair<int,int> cell = centers.at((row * maxwidth) + col);
            // 1/3 chance of no room.
            if (rand() % 3 != 0) {
                Room r(cell.first, cell.second);
            }
        }
    }

    // Position player.
    do {
        _playerRow = rand() % MAP_HEIGHT;
        _playerCol = rand() % MAP_WIDTH;
    } while (!_map[_playerRow][_playerCol]->isPassable());
}

Item* World::makeItem() {
    int r = rand() % 100;

    if (r < 50)
        return 0;
    if (r < 60)
        return new Treasure();
    if (r < 75) {
        int number = _potions[rand() % 6];
        stringstream name, article;
        if (number == 1) {
            name << "Potion";
            article << "a";
        }
        else {
            name << "Potions";
            article << number;
        }
        return new Potion(name.str(), article.str());
    }
    if (r < 80)
        return _items.at(rand() % 6)->clone();
    if (r < 84)
        return _monsters["Bat"].clone();
    if (r < 88)
        return _monsters["Rat"].clone();
    if (r < 91)
        return _monsters["Skeleton"].clone();
    if (r < 94)
        return _monsters["Ghoul"].clone();
    if (r < 96)
        return _monsters["Wight"].clone();
    if (r < 98)
        return _monsters["Vampire"].clone();
    if (r < 99)
        return _monsters["Demon"].clone();
    return _monsters["Fiend"].clone();
}

Player& World::player() {
    return *_player;
}

int World::playerRow() {
    return _playerRow;
}

void World::setPlayerRow(int row) {
    _playerRow += row;
}

int World::playerCol() {
    return _playerCol;
}

void World::setPlayerCol(int col) {
    _playerCol += col;
}

Tile& World::tileAt(int row, int col) {
    return *_map[row][col];
}

int World::height() {
    return MAP_HEIGHT;
}

int World::width() {
    return MAP_WIDTH;
}

int World::sectorHeight() {
    return MAP_SECTORHEIGHT;
}

int World::sectorWidth() {
    return MAP_SECTORWIDTH;
}

