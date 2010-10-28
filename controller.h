// Controller -- Abstract Base Class for S.Y.T.Y.C.D...O.M.G? Controllers

#ifndef CONTROLLER_H
#define CONTROLLER_H 1

#include "state.h"

struct Controller {
    virtual int   run() = 0;
    virtual STATE move_left() = 0;
    virtual STATE move_down() = 0;
    virtual STATE move_up() = 0;
    virtual STATE move_right() = 0;
    virtual STATE move_upleft() = 0;
    virtual STATE move_upright() = 0;
    virtual STATE move_downleft() = 0;
    virtual STATE move_downright() = 0;
    virtual STATE quaff() = 0;
    virtual STATE quit() = 0;
    virtual STATE refresh() = 0;
    virtual STATE resize() = 0;
    virtual STATE shell() = 0;
    virtual STATE take() = 0;
protected:
    virtual STATE command() = 0;
    virtual STATE error() = 0;
    virtual STATE fight() = 0;
    virtual STATE move(int row, int col) = 0;
};

#endif

