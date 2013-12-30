// Game -- the controller class for S.Y.T.Y.C.D...O.M.G? (implementation)
//

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
using namespace std;

#include "armor.h"
#include "direction.h"
#include "door.h"
#include "game.h"
#include "item.h"
#include "monster.h"
#include "player.h"
#include "potion.h"
#include "shield.h"
#include "treasure.h"
#include "view.h"
#include "weapon.h"
#include "world.h"

static Player player;
static View view;
static World world;

struct Game::GameImpl {
    STATE directed(string command, function<STATE(GameImpl&, int, int)> func);
    STATE close(int row, int col);
    STATE fight(int row, int col);
    STATE move(int row, int col);
    STATE open(int row, int col);
    STATE take(int row, int col);

    string _name;
    string _version;
} Game::_impl;

Game::Game()=default;

Game::~Game()=default;

int Game::run(const char *name, const char *version) {
    srand(time(NULL));

    _impl._name = name;
    _impl._version = version;

    STATE state = STATE::COMMAND;
    bool running = true;

    world.create();

    view.init();
    resize();

    Game::version();

    while(running) {
        switch(state) {
            case STATE::COMMAND:
                state = view.handleTopLevelInput(this);
                break;
            case STATE::QUIT:
                running = false;
                break;
            case STATE::ERROR:
            default:
                state = error();
                break;
        }
        view.draw(world, player);
    }

    view.pause();
    view.end();

    // Actually we won't ever get here because view.end() exit(3)s.
    return EXIT_SUCCESS;
}

STATE Game::close() {
    return _impl.directed("close", &GameImpl::close);
}

STATE Game::error() {
    view.alert();

    return STATE::COMMAND;
}

STATE Game::move_left() {
    return _impl.move( world.playerRow(), world.playerCol() - 1 );
}

STATE Game::move_down() {
    return _impl.move( world.playerRow() + 1, world.playerCol() );
}

STATE Game::move_up() {
    return _impl.move( world.playerRow() - 1, world.playerCol() );
}

STATE Game::move_right() {
    return _impl.move( world.playerRow(), world.playerCol() + 1 );
}

STATE Game::move_upleft() {
    return _impl.move( world.playerRow() - 1, world.playerCol() - 1 );
}

STATE Game::move_upright() {
    return _impl.move( world.playerRow() - 1, world.playerCol() + 1 );
}

STATE Game::move_downleft() {
    return _impl.move( world.playerRow() + 1, world.playerCol() - 1 );
}

STATE Game::move_downright() {
    return _impl.move( world.playerRow() + 1, world.playerCol() + 1 );
}

STATE Game::open() {
    return _impl.directed("open", &GameImpl::open);
}

STATE Game::quaff() {
    if (player.potions() < 1) {
        view.message("You have no potions.");
        return STATE::ERROR;
    }

    player.setPotions(-1);
    player.setHealth(+3);

    return STATE::COMMAND;
}

STATE Game::quit() {
    return STATE::QUIT;
}

STATE Game::refresh() {
    view.refresh();

    return STATE::COMMAND;
}

STATE Game::resize() {
    view.resize();
    view.draw(world, player);

    return STATE::COMMAND;
}

STATE Game::shell() {
    view.shell();

    return STATE::COMMAND;
}

// The actual keybinding is 'g' for get but we use take to avoid confusion with
// std::unique_ptr::get
STATE Game::take() {
    return _impl.directed("get item", &GameImpl::take);
}

STATE Game::version() {
    stringstream banner;

    banner <<  _impl._name << ' ' << _impl._version;
    view.message(banner.str().c_str());

    return STATE::COMMAND;
}

STATE Game::GameImpl::close(int row, int col) {
    view.message("");

    if (Door* d = dynamic_cast<Door*>(world.itemAt(row, col))) {
        if (d->open() == false) {
            view.message("The door is already closed.");
        } else {
            d->setOpen(false);
        }
        return STATE::COMMAND;
    }
    view.message("Nothing to close here.");
    return STATE::ERROR;
}

STATE Game::GameImpl::fight(int row, int col) {
    Monster* monster = dynamic_cast<Monster*>(world.itemAt(row, col));

    stringstream output;

    int damage = monster->attack();
    damage -= player.defend();

    if ( damage > 0 ) {
        player.setHealth(-damage);
        output << "The " <<  monster->name() << " hits you (" << damage <<
            " damage);  ";
    }

    if ( player.health() < 1 ) {
        return STATE::QUIT;
    }

    damage = player.attack();
    damage -= monster->defend();

    if ( damage > 0 ) {
        monster->setHealth(-damage);
        output << "You hit the "  << monster->name() << " (" << damage <<
            " damage).  ";
    }

    if ( monster->health() < 1 ) {
        world.setPlayerRow(row);
        world.setPlayerCol(col);
        output << "You kill the "  << monster->name() << ".";
        world.removeItem(row, col, true);
    }

    view.message(output.str().c_str());

    return STATE::COMMAND;
}

STATE Game::GameImpl::move(int row, int col) {
    if (row < 0
        || row >= world.height()
        || col < 0
        || col >= world.width()
    ) {
        view.message("You can't go there!");
        return STATE::COMMAND;
    }

    Tile* t = world.tileAt( row, col );

    if(t->passable() == false) {
        view.message("You can't go there!");
        return STATE::COMMAND;
    }

    Item *item = world.itemAt(row, col);

    if (Door* d = dynamic_cast<Door*>(item)) {
        if (d->open() == false) {
            view.message("The door is shut.");
            return STATE::ERROR;
        }
    }

    else if (dynamic_cast<Monster*>(item)) {
        return fight(row, col);
    }

    else if (item != nullptr) {
        return take(row, col);
    }

    world.setPlayerRow(row);
    world.setPlayerCol(col);
    view.message("");

    return STATE::COMMAND;
}

STATE Game::GameImpl::open(int row, int col) {
    view.message("");

    if (Door* d = dynamic_cast<Door*>(world.itemAt(row, col))) {
        if (d->open() == false) {
            d->setOpen(true);
        } else {
            view.message("The door is already open.");
        }
        return STATE::COMMAND;
    }

    view.message("Nothing to open here.");
    return STATE::ERROR;
}

STATE Game::GameImpl::take(int row, int col) {
    view.message("");

    Item* item = world.itemAt(row, col);

    if (item == nullptr) {
        view.message("Nothing to take here.");
        return STATE::ERROR;
    }

    if (Potion* p = dynamic_cast<Potion*>(item)) {
        player.setPotions(p->number());
        world.removeItem(row, col, true);
    }

    else if (Treasure* t = dynamic_cast<Treasure*>(item)) {
        player.setTreasure(t->amount());
        world.removeItem(row, col, true);
    }

    else if (Armor* a = dynamic_cast<Armor*>(item)) {
        if (player.armor() != nullptr) {
            view.message("You already have some armor");
            return STATE::ERROR;
        }
        player.setArmor(a);
        player.setDefense(a->defenseBonus());
        player.setOffense(a->offenseBonus());
        world.removeItem(row, col);
    }

    else if (Shield* s = dynamic_cast<Shield*>(item)) {
        if (player.shield() != nullptr) {
            view.message("You already have a shield");
            return STATE::ERROR;
        }
        player.setShield(s);
        player.setDefense(s->defenseBonus());
        player.setOffense(s->offenseBonus());
        world.removeItem(row, col);
    }

    else if (Weapon* w = dynamic_cast<Weapon*>(item)) {
        if (player.weapon() != nullptr) {
            view.message("You already have a weapon");
            return STATE::ERROR;
        }
        player.setWeapon(w);
        player.setDefense(w->defenseBonus());
        player.setOffense(w->offenseBonus());
        world.removeItem(row, col);
    }

    world.setPlayerRow(row);
    world.setPlayerCol(col);
    return STATE::COMMAND;
}

STATE Game::GameImpl::directed(string command,
    function<STATE(GameImpl&, int, int)> func) {

    stringstream prompt;
    prompt << command << " in which direction?";
    view.message(prompt.str().c_str());

    switch(view.handleDirectionInput()) {
        case DIRECTION::NORTH:
            return func(_impl, world.playerRow() - 1 , world.playerCol());
            break;
        case DIRECTION::EAST:
            return func(_impl, world.playerRow(), world.playerCol() + 1 );
            break;
        case DIRECTION::WEST:
            return func(_impl, world.playerRow(), world.playerCol() - 1 );
            break;
        case DIRECTION::SOUTH:
            return func(_impl, world.playerRow() + 1, world.playerCol());
            break;
        case DIRECTION::NORTHWEST:
            return func(_impl, world.playerRow() - 1, world.playerCol() - 1 );
            break;
        case DIRECTION::NORTHEAST:
            return func(_impl, world.playerRow() - 1, world.playerCol() + 1 );
            break;
        case DIRECTION::SOUTHWEST:
            return func(_impl, world.playerRow() + 1, world.playerCol() - 1 );
            break;
        case DIRECTION::SOUTHEAST:
            return func(_impl, world.playerRow() + 1, world.playerCol() + 1 );
            break;
        case DIRECTION::NO_DIRECTION:
        default:
            view.message("Huh?");
            return STATE::ERROR;
            break;
    }

    return STATE::COMMAND;
}

