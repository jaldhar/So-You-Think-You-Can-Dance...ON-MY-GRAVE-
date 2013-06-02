// Game -- the controller class for S.Y.T.Y.C.D...O.M.G? (interface)

#ifndef GAME_H
#define GAME_H 1

#include "state.h"

class Game {
public:
    Game();
    ~Game();
    int run(const char *name, const char *version);
    STATE close();
    STATE error();
    STATE move_left();
    STATE move_down();
    STATE move_up();
    STATE move_right();
    STATE move_upleft();
    STATE move_upright();
    STATE move_downleft();
    STATE move_downright();
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
    static GameImpl _impl;
};

#endif
