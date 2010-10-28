// sytycdomg.cc -- So You Think You Can Dance...ON MY GRAVE? (main file)
//

using namespace std;

#include <cstdlib>
#include <ctime>
#include "game.h"
#include "cursesview.h"
#include "world.h"

Game*           a = Game::instance();
CursesView*     v = CursesView::instance();
World*          w = World::instance();

int main (int, char **) {
    srand(time(NULL));

    return a->run();
}
