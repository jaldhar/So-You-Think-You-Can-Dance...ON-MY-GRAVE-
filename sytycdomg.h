// sytycdomg.h -- So You Think You Can Dance...ON MY GRAVE? (header)
//

#ifndef MAIN_H
#define MAIN_H 1

#include "game.h"
#include "cursesview.h"
#include "world.h"

struct app : public Game, CursesView, World {
    app() : Game(), CursesView(), World() { };
};

#endif
