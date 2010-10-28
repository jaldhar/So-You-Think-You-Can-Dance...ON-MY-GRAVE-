// World -- The S.Y.T.Y.C.D...O.M.G?  dungeon map and objects on it (interface)
//

#ifndef WORLD_H
#define WORLD_H 1

#include <map>
#include <string>
#include <vector>
#include "model.h"
using namespace std;

class Item;
class Monster;

class World : virtual public Model {
public:
    static World* instance();
    ~World();
    void          create();
    int           height();
    int           width();
    int           sectorHeight();
    int           sectorWidth();
    Player&       player();
    int           playerRow();
    void          setPlayerRow(int row);
    int           playerCol();
    void          setPlayerCol(int col);
    Item*         makeItem();
    Tile&         tileAt(int row, int col);

protected:
    World();

    static const int MAP_HEIGHT       = 60;
    static const int MAP_WIDTH        = 80;
    static const int MAP_SECTORHEIGHT = 20;
    static const int MAP_SECTORWIDTH  = 16;
    std::vector<Item*>              _items;
    Tile*                           _map[MAP_HEIGHT][MAP_WIDTH];
    std::map<std::string, Monster>  _monsters;
    int                             _potions[6];
    Player*                         _player;
    int                             _playerRow;
    int                             _playerCol;
    static World*                   _instance;
};

#endif
