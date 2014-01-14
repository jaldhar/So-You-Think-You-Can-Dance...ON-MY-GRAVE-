// View -- S.Y.T.Y.C.D...O.M.G? display (implementation)
//

#define _XOPEN_SOURCE 500

#include <cctype>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <functional>
#include <map>
using namespace std;

#include <curses.h>
// These ncurses macros name clash with c++ symbols on old versions of ncurses
#if NCURSES_MAJOR_VERSION < 5 || (NCURSES_MAJOR_VERSION == 5 && NCURSES_MINOR_VERSION < 9)
#undef box
#undef clear
#undef erase
#undef move
#undef refresh
#endif

#include "door.h"
#include "monster.h"
#include "terrain.h"
#include "view.h"

typedef map<int, function<STATE(Game*)>> COMMANDMAP;
typedef map<int, DIRECTION> DIRECTIONMAP;
typedef map<ITEMTYPE, TERRAIN> ITEMMAP;
typedef map<TERRAIN, chtype> TILEMAP;

static const int TILEHEIGHT = 1;
static const int TILEWIDTH  = 1;

struct View::ViewImpl {
    ViewImpl()=default;
    ~ViewImpl()=default;

    void    drawActors(World& world, int top, int left);
    void    drawItems(World& world, int top, int left, int height, int width);
    void    drawMessage();
    void    drawStatus(Player& player);
    void    drawViewport(World& world);
    void    setMessageWin(WINDOW*& win);
    void    setStatusWin(WINDOW*& win);

    static int  createMessageWin(WINDOW*, int);
    static int  createStatusWin(WINDOW*, int);
    static void end_sig(int);

    WINDOW*      _message;
    WINDOW*      _status;
    WINDOW*      _viewport;
    COMMANDMAP       _commandkeys;
    DIRECTIONMAP _directionkeys;
    ITEMMAP      _itemmap;
    TILEMAP      _tilemap;
    int          _lines;
    int          _cols;
} View::_impl;

View::View()=default;

View::~View()=default;

void View::alert() {
    beep();
}

void View::draw(World& world, Player& player) {
    curs_set(0);
    wclear(stdscr);
    _impl.drawViewport(world);
    _impl.drawStatus(player);
    _impl.drawMessage();
    doupdate();
}

void View::end() {
    curs_set(1);
    endwin();
    clear();
    exit(EXIT_SUCCESS);
}

DIRECTION View::handleDirectionInput() {
    int c = getch();

    auto it = _impl._directionkeys.find(c);
    if (it != _impl._directionkeys.end()) {
        return it->second;
    }

    return DIRECTION::NO_DIRECTION;
}

STATE View::handleTopLevelInput(Game* game) {
    int c = getch();

    auto it = _impl._commandkeys.find(c);
    if (it != _impl._commandkeys.end()) {
        return (it->second)(game);
    }

    return game->badInput();
}

void View::init() {
    setlocale(LC_ALL, "POSIX");

    struct sigaction act;
    act.sa_handler = View::ViewImpl::end_sig;
    sigemptyset (&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGSEGV, &act, NULL);

    _impl._viewport = NULL;
    _impl._lines = 0;
    _impl._cols = 0;

    _impl._commandkeys[0x12] /* CTRL-R */ = &Game::refresh;
    _impl._commandkeys[KEY_RESIZE]        = &Game::resize;
    _impl._commandkeys['h']               = &Game::move_left;
    _impl._commandkeys[KEY_LEFT]          = &Game::move_left;
    _impl._commandkeys['j']               = &Game::move_down;
    _impl._commandkeys[KEY_DOWN]          = &Game::move_down;
    _impl._commandkeys['k']               = &Game::move_up;
    _impl._commandkeys[KEY_UP]            = &Game::move_up;
    _impl._commandkeys['l']               = &Game::move_right;
    _impl._commandkeys[KEY_RIGHT]         = &Game::move_right;
    _impl._commandkeys['y']               = &Game::move_upleft;
    _impl._commandkeys[KEY_HOME]          = &Game::move_upleft;
    _impl._commandkeys['u']               = &Game::move_upright;
    _impl._commandkeys[KEY_PPAGE]         = &Game::move_upright;
    _impl._commandkeys['b']               = &Game::move_downleft;
    _impl._commandkeys[KEY_END]           = &Game::move_downleft;
    _impl._commandkeys['n']               = &Game::move_downright;
    _impl._commandkeys[KEY_NPAGE]         = &Game::move_downright;
    _impl._commandkeys['H']               = &Game::run_left;
    _impl._commandkeys['J']               = &Game::run_down;
    _impl._commandkeys['K']               = &Game::run_up;
    _impl._commandkeys['L']               = &Game::run_right;
    _impl._commandkeys['Y']               = &Game::run_upleft;
    _impl._commandkeys['U']               = &Game::run_upright;
    _impl._commandkeys['B']               = &Game::run_downleft;
    _impl._commandkeys['N']               = &Game::run_downright;
    _impl._commandkeys[0x08] /* CTRL-H */ = &Game::runadjacent_left;
    _impl._commandkeys[0x0a] /* CTRL-J */ = &Game::runadjacent_down;
    _impl._commandkeys[0x0b] /* CTRL-K */ = &Game::runadjacent_up;
    _impl._commandkeys[0x0c] /* CTRL-L */ = &Game::runadjacent_right;
    _impl._commandkeys[0x19] /* CTRL-Y */ = &Game::runadjacent_upleft;
    _impl._commandkeys[0x15] /* CTRL-U */ = &Game::runadjacent_upright;
    _impl._commandkeys[0x02] /* CTRL-B */ = &Game::runadjacent_downleft;
    _impl._commandkeys[0x0e] /* CTRL-N */ = &Game::runadjacent_downright;
    _impl._commandkeys['c']               = &Game::close;
    _impl._commandkeys['f']               = &Game::fight;
    _impl._commandkeys['F']               = &Game::fightToDeath;
    _impl._commandkeys['m']               = &Game::moveOver;
    _impl._commandkeys['M']               = &Game::runOver;
    _impl._commandkeys['o']               = &Game::open;
    _impl._commandkeys['q']               = &Game::quaff;
    _impl._commandkeys['Q']               = &Game::quit;
    _impl._commandkeys['!']               = &Game::shell;
    _impl._commandkeys[',']               = &Game::take;
    _impl._commandkeys['v']               = &Game::version;

    _impl._directionkeys['h']             = DIRECTION::WEST;
    _impl._directionkeys[KEY_LEFT]        = DIRECTION::WEST;
    _impl._directionkeys['j']             = DIRECTION::SOUTH;
    _impl._directionkeys[KEY_DOWN]        = DIRECTION::SOUTH;
    _impl._directionkeys['k']             = DIRECTION::NORTH;
    _impl._directionkeys[KEY_UP]          = DIRECTION::NORTH;
    _impl._directionkeys['l']             = DIRECTION::EAST;
    _impl._directionkeys[KEY_RIGHT]       = DIRECTION::EAST;
    _impl._directionkeys['y']             = DIRECTION::NORTHWEST;
    _impl._directionkeys[KEY_HOME]        = DIRECTION::NORTHWEST;
    _impl._directionkeys['u']             = DIRECTION::NORTHEAST;
    _impl._directionkeys[KEY_PPAGE]       = DIRECTION::NORTHEAST;
    _impl._directionkeys['b']             = DIRECTION::SOUTHWEST;
    _impl._directionkeys[KEY_END]         = DIRECTION::SOUTHWEST;
    _impl._directionkeys['n']             = DIRECTION::SOUTHEAST;
    _impl._directionkeys[KEY_NPAGE]       = DIRECTION::SOUTHEAST;
    _impl._directionkeys[0x1b]/* ESCAPE */= DIRECTION::CANCELLED;

    ripoffline(1, View::ViewImpl::createMessageWin);
    ripoffline(-1, View::ViewImpl::createStatusWin);
    initscr();
    keypad(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);     // message window
        init_pair(2, COLOR_WHITE, COLOR_BLACK);     // walls
        init_pair(3, COLOR_BLACK,  COLOR_CYAN);     // status window
        init_pair(4, COLOR_BLACK,  COLOR_BLACK);    // background
        init_pair(5, COLOR_RED,  COLOR_BLACK);      // player and items
        init_pair(6, COLOR_MAGENTA,   COLOR_BLACK); // monsters
        init_pair(7, COLOR_YELLOW,   COLOR_BLACK);  // doors
    }

    _impl._tilemap[TERRAIN::EMPTY] = ' ';    //use of ACS_* requires this goes
    _impl._tilemap[TERRAIN::CORRIDOR] = '.'; // after call to initscr().
    _impl._tilemap[TERRAIN::FLOOR]    = '.';
    _impl._tilemap[TERRAIN::C_WALL]   = '+';
    _impl._tilemap[TERRAIN::H_WALL]   = ACS_HLINE;
    _impl._tilemap[TERRAIN::V_WALL]   = ACS_VLINE;
    _impl._tilemap[TERRAIN::UL_WALL]  = ACS_ULCORNER;
    _impl._tilemap[TERRAIN::UR_WALL]  = ACS_URCORNER;
    _impl._tilemap[TERRAIN::LR_WALL]  = ACS_LRCORNER;
    _impl._tilemap[TERRAIN::LL_WALL]  = ACS_LLCORNER;
    _impl._tilemap[TERRAIN::TT_WALL]  = ACS_TTEE;
    _impl._tilemap[TERRAIN::RT_WALL]  = ACS_RTEE;
    _impl._tilemap[TERRAIN::BT_WALL]  = ACS_BTEE;
    _impl._tilemap[TERRAIN::LT_WALL]  = ACS_LTEE;
    _impl._tilemap[TERRAIN::H_DOOR_OPEN]   = '/';
    _impl._tilemap[TERRAIN::H_DOOR_CLOSED] = ACS_HLINE;
    _impl._tilemap[TERRAIN::V_DOOR_OPEN]   = '/';
    _impl._tilemap[TERRAIN::V_DOOR_CLOSED] = ACS_VLINE;
    _impl._tilemap[TERRAIN::PLAYER]   = '@';
    _impl._tilemap[TERRAIN::POTION]   = '!';
    _impl._tilemap[TERRAIN::TREASURE] = '*';
    _impl._tilemap[TERRAIN::ZOMBIE]   = 'z';
    _impl._tilemap[TERRAIN::SKELETON] = 's';
    _impl._tilemap[TERRAIN::GHOUL]    = 'g';
    _impl._tilemap[TERRAIN::WIGHT]    = 'w';
    _impl._tilemap[TERRAIN::VAMPIRE]  = 'v';
    _impl._tilemap[TERRAIN::IMP]      = 'i';
    _impl._tilemap[TERRAIN::DEMON]    = 'd';
    _impl._tilemap[TERRAIN::FIEND]    = 'f';
    _impl._tilemap[TERRAIN::SHIELD]   = ']';
    _impl._tilemap[TERRAIN::WEAPON]   = ')';
    _impl._tilemap[TERRAIN::ARMOR]    = ']';

    _impl._itemmap[ITEMTYPE::NOTHING]   = TERRAIN::EMPTY;
    _impl._itemmap[ITEMTYPE::POTION]    = TERRAIN::POTION;
    _impl._itemmap[ITEMTYPE::TREASURE]  = TERRAIN::TREASURE;
    _impl._itemmap[ITEMTYPE::ZOMBIE]    = TERRAIN::ZOMBIE;
    _impl._itemmap[ITEMTYPE::SKELETON]  = TERRAIN::SKELETON;
    _impl._itemmap[ITEMTYPE::GHOUL]     = TERRAIN::GHOUL;
    _impl._itemmap[ITEMTYPE::WIGHT]     = TERRAIN::WIGHT;
    _impl._itemmap[ITEMTYPE::VAMPIRE]   = TERRAIN::VAMPIRE;
    _impl._itemmap[ITEMTYPE::IMP]       = TERRAIN::IMP;
    _impl._itemmap[ITEMTYPE::DEMON]     = TERRAIN::DEMON;
    _impl._itemmap[ITEMTYPE::FIEND]     = TERRAIN::FIEND;
    _impl._itemmap[ITEMTYPE::BUCKLER]   = TERRAIN::SHIELD;
    _impl._itemmap[ITEMTYPE::SHIELD]    = TERRAIN::SHIELD;
    _impl._itemmap[ITEMTYPE::SWORD]     = TERRAIN::WEAPON;
    _impl._itemmap[ITEMTYPE::AXE]       = TERRAIN::WEAPON;
    _impl._itemmap[ITEMTYPE::CHAINMAIL] = TERRAIN::ARMOR;
    _impl._itemmap[ITEMTYPE::PLATEMAIL] = TERRAIN::ARMOR;
}

void View::message(const char *msg) {
    wclear(_impl._message);
    mvwaddstr(_impl._message, 0, 0, msg);
    wrefresh(_impl._message);
}

void View::pause() {
    message("--press space to continue--");
    int c;
    do {
        c = getch();
    } while (c != ' ');
}

void View::refresh() {
    redrawwin(_impl._status);
    redrawwin(_impl._message);
    redrawwin(_impl._viewport);
    doupdate();
}

void View::resize() {
    getmaxyx(stdscr, _impl._lines, _impl._cols);

    wbkgd(stdscr, ' ');

    if (_impl._viewport == NULL) {
        cbreak();
        noecho();
        nonl();
        _impl._viewport = subwin(stdscr, _impl._lines, _impl._cols, 0, 0);
    } else {
        wresize(_impl._viewport, _impl._lines, _impl._cols);
    }
    wbkgd(_impl._viewport, ' ' | COLOR_PAIR(4));

    wresize(_impl._message, 1, _impl._cols);
    wbkgd(_impl._message, ' ' | COLOR_PAIR(1));

    wresize(_impl._status, 1, _impl._cols);
    wbkgd(_impl._status, ' ' | COLOR_PAIR(3));
}

void View::shell() {
    def_prog_mode();
    endwin();
    fprintf(stderr, "Type 'exit' to return.\n");
    int returncode = system("/bin/sh");
    returncode += 0; // stops g++ warning for set but unused variable.
    reset_prog_mode();
}

// Private methods

int View::ViewImpl::createMessageWin(WINDOW* win, int /* ncols */) {
    _impl.setMessageWin(win);

    return 0;
}

int View::ViewImpl::createStatusWin(WINDOW* win, int /* ncols */) {
    _impl.setStatusWin(win);

    return 0;
}

void View::ViewImpl::drawActors(World& world, int top, int left) {
    mvwaddch(_viewport, world.playerRow() - top, world.playerCol() - left,
        _tilemap[TERRAIN::PLAYER] | COLOR_PAIR(5) | A_BOLD);
}

void View::ViewImpl::drawItems(World& world, int top, int left, int height, int width) {
    world.foreach_item(top, left, height, width, [=](int row, int col, ITEMPTR& item) {
        TERRAIN t;

        switch(item->type()) {
            case ITEMTYPE::DOOR: {
                    auto d = dynamic_cast<Door*>(item.get());
                    if (d->open()) {
                        t = (d->horizontal()) ? TERRAIN::H_DOOR_OPEN
                            : TERRAIN::V_DOOR_OPEN;
                    } else {
                        t = (d->horizontal()) ? TERRAIN::H_DOOR_CLOSED
                            : TERRAIN::V_DOOR_CLOSED;
                    }
                }
                break;
            default:
                t = _itemmap[item->type()];
                break;
                break;
        }
        mvwaddch(_viewport, row - top, col - left, _tilemap[t] | COLOR_PAIR(7));
    });
}

void View::ViewImpl::drawMessage() {
    wnoutrefresh(_message);
}

void View::ViewImpl::drawStatus(Player& player) {
    wclear(_status);
    mvwprintw(_status, 0, 1,
        "H: %2d    P: %2d    T: %2d    %s/%s/%s",
        player.health(), player.potions(), player.treasure(),
        player.weapon() != nullptr ? player.weapon()->name().c_str() : "no weapon",
        player.shield() != nullptr ? player.shield()->name().c_str() : "no shield",
        player.armor() != nullptr ? player.armor()->name().c_str() : "no armor"
    );

    wnoutrefresh(_status);
}

void View::ViewImpl::drawViewport(World& world) {
    int screenHeight, screenWidth;
    getmaxyx(_viewport, screenHeight, screenWidth);

    wbkgd(_viewport, _tilemap[TERRAIN::EMPTY]);

    int playerCol = world.playerCol();
    int playerRow = world.playerRow();
    int worldHeight = world.height();
    int worldWidth  = world.width();

    int top = playerRow - (screenHeight + 1) / 2;
    int left = playerCol - (screenWidth + 1) / 2;

    for (int row = 0; row < screenHeight; row += TILEHEIGHT) {
        int mapRow = row + top;

        if (mapRow < 0 || mapRow >= worldHeight) {
            continue;
        }

        for (int  col = 0; col < screenWidth; col += TILEWIDTH) {
            int mapCol = col + left;

            if (mapCol < 0 || mapCol >= worldWidth) {
                continue;
            }

            chtype display;
            Tile* t = world.tileAt(mapRow, mapCol);

            if (t->visible()) {
                display = _tilemap[t->terrain()];

                if (t->isBlock()) {
                    display |= COLOR_PAIR(2);
                }

            } else {
                continue;
            }

            mvwaddch(_viewport, row, col, display);
        }
    }

    drawItems(world, top, left, screenHeight, screenWidth);

    drawActors(world, top, left);

    wnoutrefresh(_viewport);
}

void View::ViewImpl::end_sig(int /* sig */) {
    View view;
    view.end();
}

void View::ViewImpl::setMessageWin(WINDOW*& win) {

    _message = win;
}

void View::ViewImpl::setStatusWin(WINDOW*& win) {

    _status = win;
}

