// Game -- the controller class for So You Think You Can Dance...ON MY GRAVE?
// (interface)

#ifndef GAME_H
#define GAME_H 1

#include "controller.h"

class Game : virtual public Controller {
public:
    int   run();
    STATE move_left();
    STATE move_down();
    STATE move_up();
    STATE move_right();
    STATE move_upleft();
    STATE move_upright();
    STATE move_downleft();
    STATE move_downright();
    STATE quaff();
    STATE quit();
    STATE refresh();
    STATE resize();
    STATE shell();
    STATE take();
protected:
    STATE command();
    STATE error();
    STATE fight();
    STATE move(int row, int col);

public:
    static Game* instance();
    ~Game();

protected:
    Game();
    static Game* _instance;
};

#endif
