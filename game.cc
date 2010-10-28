// Game -- the controller class for S.Y.T.Y.C.D...O.M.G? (implementation)
//

#include <cstring>
#include <sstream>
using namespace std;

#include "sytycdomg.h"
#include "cursesview.h"
#include "item.h"
#include "monster.h"
#include "player.h"
#include "tile.h"
#include "take.h"
#include "world.h"

extern CursesView*     v;
extern World*          w;

Game* Game::_instance = 0;

Game* Game::instance(){
    if(_instance == 0)
        _instance = new Game();

    return _instance;
}

Game::Game() {
}

Game::~Game() {
    if(_instance != 0)
            delete _instance;
}

int Game::run() {
    STATE phase = COMMAND;
    w->create();
    v->init();

    while(1) {
        switch (phase) {
            case COMMAND:
                phase = v->handleInput();
                break;
            case FIGHT:
                phase = fight();
                break;
            case QUIT:
                goto end;
            case ERROR:
            default:
                phase = error();
        }
        v->draw();
    }

end:
    v->pause();
    v->end();

    return EXIT_SUCCESS;
}

STATE  Game::command() {

    return COMMAND;
}

STATE Game::error() {

    return COMMAND;
}

STATE Game::fight() {
    Monster* monster =
        dynamic_cast<Monster *>(w->tileAt( w->playerRow(), w->playerCol() ).contents());
    Player& p = w->player();

    while (1) {
        int damage = monster->attack();
        damage -= p.defend();
        stringstream msg;
        if ( damage > 0 ) {
            p.setHealth(-damage);
        }

        if ( p.health() < 1 ) {
            return QUIT;
        }

        damage = p.attack();
        damage -= monster->defend();
        if ( damage > 0 ) {
            monster->setHealth(-damage);
        }

        if ( monster->health() < 1 ) {
            w->tileAt( w->playerRow(), w->playerCol() ).setContents(0);
            break;
        }

    }

    return COMMAND;
}

STATE Game::move_left() {
    return move( 0, -1 );
}

STATE Game::move_down() {
    return move( 1, 0 );
}

STATE Game::move_up() {
    return move( -1, 0 );
}

STATE Game::move_right() {
    return move( 0, 1 );
}

STATE Game::move_upleft() {
    return move( -1, -1 );
}

STATE Game::move_upright() {
    return move( -1, 1 );
}

STATE Game::move_downleft() {
    return move( 1, -1 );
}

STATE Game::move_downright() {
    return move( 1, 1 );
}

STATE Game::move(int row, int col) {

    int temprow = w->playerRow() + row;
    int tempcol = w->playerCol() + col;
    if (temprow > -1
        && temprow < w->height()
        && tempcol > 1
        && tempcol < w->width()
        && w->tileAt( temprow, tempcol ).isPassable())
    {
        w->setPlayerRow(row);
        w->setPlayerCol(col);
        Item *contents = w->tileAt( w->playerRow(), w->playerCol() ).contents();
        if ( contents != 0 ) {
            if(dynamic_cast<Monster*>(contents))
                return FIGHT;
        }
    }

    return COMMAND;
}

STATE Game::quaff() {
    Player& p = w->player();
    if (p.potions() > 0) {
        p.setPotions(-1);
        p.setHealth(+3);
    }
    return COMMAND;
}

STATE Game::quit() {
    return QUIT;
}

STATE Game::refresh() {
    v->refresh();

    return COMMAND;
}

STATE Game::resize() {
    v->resize();

    return COMMAND;
}

STATE Game::shell() {
    v->shell();

    return COMMAND;
}

STATE Game::take() {
    Tile& tile = w->tileAt(w->playerRow(), w->playerCol());
    Player& p = w->player();
    Item* contents = tile.contents();

    if(!contents || !dynamic_cast<Take*>(contents)->take(p, tile)) {
        /* Nothing to take here. */
    }

    return COMMAND;
}

