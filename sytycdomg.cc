// sytycdomg.cc -- So You Think You Can Dance...ON MY GRAVE? (main file)
//

#include "game.h"
#include "player.h"

const char *name = "So You Think You Can Dance...ON MY GRAVE?";
const char *version = "2.0";

Player player;

int main (int, char **) {
    Game game;

    return game.run(name, version);
}
