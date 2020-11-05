// Game -- the controller class for S.Y.T.Y.C.D...O.M.G? (interface)

#ifndef GAME_H
#define GAME_H 1

#include "state.h"

class Game {
public:
    Game();
    ~Game();
    int run(const char *name, const char *version);
    STATE badInput();
    STATE close();
    STATE error();
    STATE fight();
    STATE fightToDeath();
    STATE move_left();
    STATE move_down();
    STATE move_up();
    STATE move_right();
    STATE move_upleft();
    STATE move_upright();
    STATE move_downleft();
    STATE move_downright();
    STATE run_left();
    STATE run_down();
    STATE run_up();
    STATE run_right();
    STATE run_upleft();
    STATE run_upright();
    STATE run_downleft();
    STATE run_downright();
    STATE runadjacent_left();
    STATE runadjacent_down();
    STATE runadjacent_up();
    STATE runadjacent_right();
    STATE runadjacent_upleft();
    STATE runadjacent_upright();
    STATE runadjacent_downleft();
    STATE runadjacent_downright();
    STATE moveOver();
    STATE runOver();
    STATE open();
    STATE quaff();
    STATE quit();
    STATE refresh();
    STATE resize();
    STATE shell();
    STATE take();
    STATE version();

private:
    struct GameImpl;
    static GameImpl impl_;
};

#endif
