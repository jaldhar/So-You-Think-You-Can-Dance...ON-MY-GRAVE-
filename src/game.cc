// Game -- the controller class for S.Y.T.Y.C.D...O.M.G? (implementation)
//

#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>

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
    GameImpl();

    STATE close();
    STATE fight();
    STATE fightHere(int row, int col, Monster*& monster);
    bool  canMove(int row, int col);
    bool  itemIsAdjacent(int row, int col);
    STATE move();
    STATE open();
    STATE take();
    STATE takeHere(int row, int col, Item*& item);
    STATE directed(std::string command, std::function<STATE(GameImpl&)> func);

    std::string name_;
    std::string version_;
    bool   keepfighting_;
    bool   keepmoving_;
    bool   pickup_;
    bool   runadjacent_;
} Game::impl_;

Game::Game()=default;

Game::~Game()=default;

int Game::run(const char *name, const char *version) {
    std::srand(std::time(NULL));

    impl_.name_ = name;
    impl_.version_ = version;
    impl_.keepfighting_ = false;
    impl_.keepmoving_ = false;
    impl_.pickup_ = false;
    impl_.runadjacent_ = false;

    STATE state = STATE::COMMAND;
    bool running = true;

    world.create();

    view.init();
    resize();

    Game::version();

    while(running) {
        switch(state) {
            case STATE::COMMAND:
                impl_.keepfighting_ = false;
                impl_.keepmoving_ = false;
                impl_.pickup_ = false;
                impl_.runadjacent_ = false;
                state = view.handleTopLevelInput(this);
                break;
            case STATE::FIGHTING:
                impl_.keepmoving_ = false;
                impl_.pickup_ = false;
                impl_.runadjacent_ = false;
                state = impl_.fight();
                break;
            case STATE::MOVING:
                impl_.keepfighting_ = false;
                state = impl_.move();
                break;
            case STATE::QUIT:
                impl_.keepfighting_ = false;
                impl_.keepmoving_ = false;
                impl_.pickup_ = false;
                impl_.runadjacent_ = false;
                running = false;
                break;
            case STATE::ERROR:
            default:
                impl_.keepfighting_ = false;
                impl_.keepmoving_ = false;
                impl_.pickup_ = false;
                impl_.runadjacent_ = false;
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

STATE Game::badInput() {
    view.message("Huh?");
    return STATE::ERROR;
}

STATE Game::close() {
    return impl_.directed("close", &GameImpl::close);
}

STATE Game::error() {
    view.alert();

    return STATE::COMMAND;
}

STATE Game::fight() {
    impl_.keepfighting_ = false;
    return impl_.directed("fight", &GameImpl::fight);
}

STATE Game::fightToDeath() {
    impl_.keepfighting_ = true;
    return impl_.directed("fight to the death", &GameImpl::fight);
}

STATE Game::move_left() {
    player.setFacingY(0);
    player.setFacingX(-1);
    impl_.keepmoving_ = false;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::move_down() {
    player.setFacingY(1);
    player.setFacingX(0);
    impl_.keepmoving_ = false;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::move_up() {
    player.setFacingY(-1);
    player.setFacingX(0);
    impl_.keepmoving_ = false;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::move_right() {
    player.setFacingY(0);
    player.setFacingX(1);
    impl_.keepmoving_ = false;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::move_upleft() {
    player.setFacingY(-1);
    player.setFacingX(-1);
    impl_.keepmoving_ = false;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::move_upright() {
    player.setFacingY(-1);
    player.setFacingX(1);
    impl_.keepmoving_ = false;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::move_downleft() {
    player.setFacingY(1);
    player.setFacingX(-1);
    impl_.keepmoving_ = false;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::move_downright() {
    player.setFacingY(1);
    player.setFacingX(1);
    impl_.keepmoving_ = false;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::run_left() {
    player.setFacingY(0);
    player.setFacingX(-1);
    impl_.keepmoving_ = true;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::run_down() {
    player.setFacingY(1);
    player.setFacingX(0);
    impl_.keepmoving_ = true;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::run_up() {
    player.setFacingY(-1);
    player.setFacingX(0);
    impl_.keepmoving_ = true;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::run_right() {
    player.setFacingY(0);
    player.setFacingX(1);
    impl_.keepmoving_ = true;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::run_upleft() {
    player.setFacingY(-1);
    player.setFacingX(-1);
    impl_.keepmoving_ = true;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::run_upright() {
    player.setFacingY(-1);
    player.setFacingX(1);
    impl_.keepmoving_ = true;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::run_downleft() {
    player.setFacingY(1);
    player.setFacingX(-1);
    impl_.keepmoving_ = true;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::run_downright() {
    player.setFacingY(1);
    player.setFacingX(1);
    impl_.keepmoving_ = true;
    impl_.pickup_ = true;
    impl_.runadjacent_ = false;
    return STATE::MOVING;
}

STATE Game::runadjacent_left() {
    player.setFacingY(0);
    player.setFacingX(-1);
    impl_.keepmoving_ = true;
    impl_.pickup_ = false;
    impl_.runadjacent_ = true;
    return STATE::MOVING;
}

STATE Game::runadjacent_down() {
    player.setFacingY(1);
    player.setFacingX(0);
    impl_.keepmoving_ = true;
    impl_.pickup_ = false;
    impl_.runadjacent_ = true;
    return STATE::MOVING;
}

STATE Game::runadjacent_up() {
    player.setFacingY(-1);
    player.setFacingX(0);
    impl_.keepmoving_ = true;
    impl_.pickup_ = false;
    impl_.runadjacent_ = true;
    return STATE::MOVING;
}

STATE Game::runadjacent_right() {
    player.setFacingY(0);
    player.setFacingX(1);
    impl_.keepmoving_ = true;
    impl_.pickup_ = false;
    impl_.runadjacent_ = true;
    return STATE::MOVING;
}

STATE Game::runadjacent_upleft() {
    player.setFacingY(-1);
    player.setFacingX(-1);
    impl_.keepmoving_ = true;
    impl_.pickup_ = false;
    impl_.runadjacent_ = true;
    return STATE::MOVING;
}

STATE Game::runadjacent_upright() {
    player.setFacingY(-1);
    player.setFacingX(1);
    impl_.keepmoving_ = true;
    impl_.pickup_ = false;
    impl_.runadjacent_ = true;
    return STATE::MOVING;
}

STATE Game::runadjacent_downleft() {
    player.setFacingY(1);
    player.setFacingX(-1);
    impl_.keepmoving_ = true;
    impl_.pickup_ = false;
    impl_.runadjacent_ = true;
    return STATE::MOVING;
}

STATE Game::runadjacent_downright() {
    player.setFacingY(1);
    player.setFacingX(1);
    impl_.keepmoving_ = true;
    impl_.pickup_ = false;
    impl_.runadjacent_ = true;
    return STATE::MOVING;
}

STATE Game::moveOver() {
    impl_.keepmoving_ = false;
    impl_.pickup_ = false;
    return impl_.directed("move over", &GameImpl::move);
}

STATE Game::runOver() {
    impl_.keepmoving_ = true;
    impl_.pickup_ = false;
    return impl_.directed("run over", &GameImpl::move);
}

STATE Game::open() {
    return impl_.directed("open", &GameImpl::open);
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

STATE Game::take() {
    return impl_.take();
}

STATE Game::version() {
    std::stringstream banner;

    banner <<  impl_.name_ << ' ' << impl_.version_;
    view.message(banner.str().c_str());

    return STATE::COMMAND;
}

Game::GameImpl::GameImpl() : name_{}, version_{}, keepfighting_{false}, 
keepmoving_{false}, pickup_{false}, runadjacent_{false} {
}

STATE Game::GameImpl::close() {
    int row = world.playerRow() + player.facingY();
    int col = world.playerCol() + player.facingX();
    view.message("");

    if (Door* door = dynamic_cast<Door*>(world.itemAt(row, col))) {
        if (door->open() == false) {
            view.message("The door is already closed.");
            return STATE::ERROR;
        } else {
            door->setOpen(false);
        }
        return STATE::COMMAND;
    }
    view.message("Nothing to close here.");
    return STATE::ERROR;
}

STATE Game::GameImpl::fight() {
    int row = world.playerRow() + player.facingY();
    int col = world.playerCol() + player.facingX();
    if (Monster* monster = dynamic_cast<Monster*>(world.itemAt(row, col))) {
        return fightHere(row, col, monster);
    }
    view.message("Nothing to fight here.");
    return STATE::ERROR;
}

STATE Game::GameImpl::fightHere(int row, int col, Monster*& monster) {
    std::stringstream output;

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

    STATE result = keepfighting_ ? STATE::FIGHTING : STATE::COMMAND;

    if ( monster->health() < 1 ) {
        world.setPlayerRow(row);
        world.setPlayerCol(col);
        output << "You kill the "  << monster->name() << ".";
        world.removeItem(row, col, true);
        result = STATE::COMMAND;
    }

    view.message(output.str().c_str());

    return result;
}

bool Game::GameImpl::canMove(int row, int col) {
    if (row < 0
        || row >= world.height()
        || col < 0
        || col >= world.width()) {
        return false;
    }

    Tile* t = world.tileAt( row, col );
    if(t->passable() == false) {
        return false;
    }

    return true;
}

bool Game::GameImpl::itemIsAdjacent(int row, int col) {
    for (int y = row - 1; y <= row + 1; y++) {
        for (int x = col - 1; x <= col + 1; x++) {
            if (world.itemAt(y, x) != nullptr) {
                return true;
            }
        }
    }
    return false;
}

STATE Game::GameImpl::move() {
    int row = world.playerRow() + player.facingY();
    int col = world.playerCol() + player.facingX();
    if(canMove(row, col) == false) {
        if (keepmoving_ == false) {
            view.message("You can't go there!");
        }
        return STATE::ERROR;
    }

    if (Item *item = world.itemAt(row, col)) {

        if (Door* d = dynamic_cast<Door*>(item)) {
            if (d->open() == false) {
                view.message("The door is shut.");
                return STATE::ERROR;
            }
        }

        else if (Monster* monster = dynamic_cast<Monster*>(item)) {
            return fightHere(row, col, monster);
        }

        else {
            if (pickup_) {
                return takeHere(row, col, item);
            }
        }
    }

    world.setPlayerRow(row);
    world.setPlayerCol(col);
    view.message("");

    if (runadjacent_ && itemIsAdjacent(row, col)) {
        return STATE::COMMAND;
    }

    return keepmoving_ ? STATE::MOVING : STATE::COMMAND;
}

STATE Game::GameImpl::open() {
    int row = world.playerRow() + player.facingY();
    int col = world.playerCol() + player.facingX();
    view.message("");

    if (Door* door = dynamic_cast<Door*>(world.itemAt(row, col))) {
        if (door->open() == true) {
            view.message("The door is already open.");
            return STATE::ERROR;
        } else {
            door->setOpen(true);
        }
        return STATE::COMMAND;
    }
    view.message("Nothing to open here.");
    return STATE::ERROR;
}

STATE Game::GameImpl::take() {
    int row = world.playerRow();
    int col = world.playerCol();
    view.message("");

    Item* item = world.itemAt(row, col);

    if (item != nullptr) {
        return takeHere(row, col, item);
    }
    view.message("Nothing to take here.");
    return STATE::ERROR;
}

STATE Game::GameImpl::takeHere(int row, int col, Item*& item) {
    if (Potion* p = dynamic_cast<Potion*>(item)) {
        player.setPotions(p->number());
    }

    else if (Treasure* t = dynamic_cast<Treasure*>(item)) {
        player.setTreasure(t->amount());
    }

    else if (Armor* a = dynamic_cast<Armor*>(item)) {
        if (player.armor() != nullptr) {
            view.message("You already have some armor");
            return STATE::ERROR;
        }
        player.setArmor(a);
        player.setDefense(a->defenseBonus());
        player.setOffense(a->offenseBonus());
    }

    else if (Shield* s = dynamic_cast<Shield*>(item)) {
        if (player.shield() != nullptr) {
            view.message("You already have a shield");
            return STATE::ERROR;
        }
        player.setShield(s);
        player.setDefense(s->defenseBonus());
        player.setOffense(s->offenseBonus());
    }

    else if (Weapon* w = dynamic_cast<Weapon*>(item)) {
        if (player.weapon() != nullptr) {
            view.message("You already have a weapon");
            return STATE::ERROR;
        }
        player.setWeapon(w);
        player.setDefense(w->defenseBonus());
        player.setOffense(w->offenseBonus());
    }

    else {
        view.message("You can't take that!");
        return STATE::ERROR;
    }

    world.removeItem(row, col);
    world.setPlayerRow(row);
    world.setPlayerCol(col);
    return STATE::COMMAND;
}

STATE Game::GameImpl::directed(std::string command,
    std::function<STATE(GameImpl&)> func) {

    std::stringstream prompt;
    prompt << command << " in which direction?";
    view.message(prompt.str().c_str());

    switch(view.handleDirectionInput()) {
        case DIRECTION::NORTH:
            player.setFacingY(-1);
            player.setFacingX(0);
            return func(impl_);
            break;
        case DIRECTION::EAST:
            player.setFacingY(0);
            player.setFacingX(1);
            return func(impl_);
            break;
        case DIRECTION::WEST:
            player.setFacingY(0);
            player.setFacingX(-1);
            return func(impl_);
            break;
        case DIRECTION::SOUTH:
            player.setFacingY(1);
            player.setFacingX(0);
            return func(impl_);
            break;
        case DIRECTION::NORTHWEST:
            player.setFacingY(-1);
            player.setFacingX(-1);
            return func(impl_);
            break;
        case DIRECTION::NORTHEAST:
            player.setFacingY(-1);
            player.setFacingX(1);
            return func(impl_);
            break;
        case DIRECTION::SOUTHWEST:
            player.setFacingY(1);
            player.setFacingX(-1);
            return func(impl_);
            break;
        case DIRECTION::SOUTHEAST:
            player.setFacingY(1);
            player.setFacingX(1);
            return func(impl_);
            break;
        case DIRECTION::CANCELLED:
            view.message("");
            return STATE::COMMAND;
            break;
        case DIRECTION::NO_DIRECTION:
        default:
            view.message("Huh?");
            return STATE::ERROR;
            break;
    }

    return STATE::COMMAND;
}

