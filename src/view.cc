// View -- S.Y.T.Y.C.D...O.M.G? display (implementation)
//

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif

#include <cctype>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <functional>
#include <map>

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

struct WindowDeleter {
    void operator()(WINDOW* window) {
        delwin(window);
    }
};

using WindowPtr = std::unique_ptr<WINDOW, WindowDeleter>;
using CommandMap = std::map<int, std::function<STATE(Game*)>>;
using DirectionMap = std::map<int, DIRECTION>;
using ItemMap = std::map<ITEMTYPE, TERRAIN>;
using TileMap = std::map<TERRAIN, chtype>;

constexpr int TILEHEIGHT = 1;
constexpr int TILEWIDTH  = 1;

struct View::ViewImpl {
    ViewImpl();
    ~ViewImpl()=default;

    void    drawActors(World& world, int top, int left);
    void    drawItems(World& world, int top, int left, int height, int width);
    void    drawMessage();
    void    drawStatus(Player& player);
    void    drawViewport(World& world);
    void    setMessageWin(WINDOW* win);
    void    setStatusWin(WINDOW* win);

    static int  createMessageWin(WINDOW*, int);
    static int  createStatusWin(WINDOW*, int);
    static void end_sig(int);

    WindowPtr      message_;
    WindowPtr      status_;
    WindowPtr      viewport_;
    CommandMap   commandkeys_;
    DirectionMap directionkeys_;
    ItemMap      itemmap_;
    TileMap      tilemap_;
    int          lines_;
    int          cols_;
} View::impl_;

View::View()=default;

View::~View()=default;

void View::alert() {
    beep();
}

void View::draw(World& world, Player& player) {
    curs_set(0);
    wclear(stdscr);
    impl_.drawViewport(world);
    impl_.drawStatus(player);
    impl_.drawMessage();
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

    auto it = impl_.directionkeys_.find(c);
    if (it != impl_.directionkeys_.end()) {
        return it->second;
    }

    return DIRECTION::NO_DIRECTION;
}

STATE View::handleTopLevelInput(Game* game) {
    int c = getch();

    auto it = impl_.commandkeys_.find(c);
    if (it != impl_.commandkeys_.end()) {
        return (it->second)(game);
    }

    return game->badInput();
}

void View::init() {
    std::setlocale(LC_ALL, "POSIX");

    struct sigaction act;
    act.sa_handler = View::ViewImpl::end_sig;
    sigemptyset (&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGSEGV, &act, NULL);

    impl_.viewport_ = NULL;
    impl_.lines_ = 0;
    impl_.cols_ = 0;

    impl_.commandkeys_[0x12] /* CTRL-R */ = &Game::refresh;
    impl_.commandkeys_[KEY_RESIZE]        = &Game::resize;
    impl_.commandkeys_['h']               = &Game::move_left;
    impl_.commandkeys_[KEY_LEFT]          = &Game::move_left;
    impl_.commandkeys_['j']               = &Game::move_down;
    impl_.commandkeys_[KEY_DOWN]          = &Game::move_down;
    impl_.commandkeys_['k']               = &Game::move_up;
    impl_.commandkeys_[KEY_UP]            = &Game::move_up;
    impl_.commandkeys_['l']               = &Game::move_right;
    impl_.commandkeys_[KEY_RIGHT]         = &Game::move_right;
    impl_.commandkeys_['y']               = &Game::move_upleft;
    impl_.commandkeys_[KEY_HOME]          = &Game::move_upleft;
    impl_.commandkeys_['u']               = &Game::move_upright;
    impl_.commandkeys_[KEY_PPAGE]         = &Game::move_upright;
    impl_.commandkeys_['b']               = &Game::move_downleft;
    impl_.commandkeys_[KEY_END]           = &Game::move_downleft;
    impl_.commandkeys_['n']               = &Game::move_downright;
    impl_.commandkeys_[KEY_NPAGE]         = &Game::move_downright;
    impl_.commandkeys_['H']               = &Game::run_left;
    impl_.commandkeys_['J']               = &Game::run_down;
    impl_.commandkeys_['K']               = &Game::run_up;
    impl_.commandkeys_['L']               = &Game::run_right;
    impl_.commandkeys_['Y']               = &Game::run_upleft;
    impl_.commandkeys_['U']               = &Game::run_upright;
    impl_.commandkeys_['B']               = &Game::run_downleft;
    impl_.commandkeys_['N']               = &Game::run_downright;
    impl_.commandkeys_[0x08] /* CTRL-H */ = &Game::runadjacent_left;
    impl_.commandkeys_[0x0a] /* CTRL-J */ = &Game::runadjacent_down;
    impl_.commandkeys_[0x0b] /* CTRL-K */ = &Game::runadjacent_up;
    impl_.commandkeys_[0x0c] /* CTRL-L */ = &Game::runadjacent_right;
    impl_.commandkeys_[0x19] /* CTRL-Y */ = &Game::runadjacent_upleft;
    impl_.commandkeys_[0x15] /* CTRL-U */ = &Game::runadjacent_upright;
    impl_.commandkeys_[0x02] /* CTRL-B */ = &Game::runadjacent_downleft;
    impl_.commandkeys_[0x0e] /* CTRL-N */ = &Game::runadjacent_downright;
    impl_.commandkeys_['c']               = &Game::close;
    impl_.commandkeys_['f']               = &Game::fight;
    impl_.commandkeys_['F']               = &Game::fightToDeath;
    impl_.commandkeys_['m']               = &Game::moveOver;
    impl_.commandkeys_['M']               = &Game::runOver;
    impl_.commandkeys_['o']               = &Game::open;
    impl_.commandkeys_['q']               = &Game::quaff;
    impl_.commandkeys_['Q']               = &Game::quit;
    impl_.commandkeys_['!']               = &Game::shell;
    impl_.commandkeys_[',']               = &Game::take;
    impl_.commandkeys_['v']               = &Game::version;

    impl_.directionkeys_['h']             = DIRECTION::WEST;
    impl_.directionkeys_[KEY_LEFT]        = DIRECTION::WEST;
    impl_.directionkeys_['j']             = DIRECTION::SOUTH;
    impl_.directionkeys_[KEY_DOWN]        = DIRECTION::SOUTH;
    impl_.directionkeys_['k']             = DIRECTION::NORTH;
    impl_.directionkeys_[KEY_UP]          = DIRECTION::NORTH;
    impl_.directionkeys_['l']             = DIRECTION::EAST;
    impl_.directionkeys_[KEY_RIGHT]       = DIRECTION::EAST;
    impl_.directionkeys_['y']             = DIRECTION::NORTHWEST;
    impl_.directionkeys_[KEY_HOME]        = DIRECTION::NORTHWEST;
    impl_.directionkeys_['u']             = DIRECTION::NORTHEAST;
    impl_.directionkeys_[KEY_PPAGE]       = DIRECTION::NORTHEAST;
    impl_.directionkeys_['b']             = DIRECTION::SOUTHWEST;
    impl_.directionkeys_[KEY_END]         = DIRECTION::SOUTHWEST;
    impl_.directionkeys_['n']             = DIRECTION::SOUTHEAST;
    impl_.directionkeys_[KEY_NPAGE]       = DIRECTION::SOUTHEAST;
    impl_.directionkeys_[0x1b]/* ESCAPE */= DIRECTION::CANCELLED;

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

    impl_.tilemap_[TERRAIN::EMPTY] = ' ';    //use of ACS_* requires this goes
    impl_.tilemap_[TERRAIN::CORRIDOR] = '.'; // after call to initscr().
    impl_.tilemap_[TERRAIN::FLOOR]    = '.';
    impl_.tilemap_[TERRAIN::C_WALL]   = '+';
    impl_.tilemap_[TERRAIN::H_WALL]   = ACS_HLINE;
    impl_.tilemap_[TERRAIN::V_WALL]   = ACS_VLINE;
    impl_.tilemap_[TERRAIN::UL_WALL]  = ACS_ULCORNER;
    impl_.tilemap_[TERRAIN::UR_WALL]  = ACS_URCORNER;
    impl_.tilemap_[TERRAIN::LR_WALL]  = ACS_LRCORNER;
    impl_.tilemap_[TERRAIN::LL_WALL]  = ACS_LLCORNER;
    impl_.tilemap_[TERRAIN::TT_WALL]  = ACS_TTEE;
    impl_.tilemap_[TERRAIN::RT_WALL]  = ACS_RTEE;
    impl_.tilemap_[TERRAIN::BT_WALL]  = ACS_BTEE;
    impl_.tilemap_[TERRAIN::LT_WALL]  = ACS_LTEE;
    impl_.tilemap_[TERRAIN::H_DOOR_OPEN]   = '/';
    impl_.tilemap_[TERRAIN::H_DOOR_CLOSED] = ACS_HLINE;
    impl_.tilemap_[TERRAIN::V_DOOR_OPEN]   = '/';
    impl_.tilemap_[TERRAIN::V_DOOR_CLOSED] = ACS_VLINE;
    impl_.tilemap_[TERRAIN::PLAYER]   = '@';
    impl_.tilemap_[TERRAIN::POTION]   = '!';
    impl_.tilemap_[TERRAIN::TREASURE] = '*';
    impl_.tilemap_[TERRAIN::ZOMBIE]   = 'z';
    impl_.tilemap_[TERRAIN::SKELETON] = 's';
    impl_.tilemap_[TERRAIN::GHOUL]    = 'g';
    impl_.tilemap_[TERRAIN::WIGHT]    = 'w';
    impl_.tilemap_[TERRAIN::VAMPIRE]  = 'v';
    impl_.tilemap_[TERRAIN::IMP]      = 'i';
    impl_.tilemap_[TERRAIN::DEMON]    = 'd';
    impl_.tilemap_[TERRAIN::FIEND]    = 'f';
    impl_.tilemap_[TERRAIN::SHIELD]   = ']';
    impl_.tilemap_[TERRAIN::WEAPON]   = ')';
    impl_.tilemap_[TERRAIN::ARMOR]    = ']';

    impl_.itemmap_[ITEMTYPE::NOTHING]   = TERRAIN::EMPTY;
    impl_.itemmap_[ITEMTYPE::POTION]    = TERRAIN::POTION;
    impl_.itemmap_[ITEMTYPE::TREASURE]  = TERRAIN::TREASURE;
    impl_.itemmap_[ITEMTYPE::ZOMBIE]    = TERRAIN::ZOMBIE;
    impl_.itemmap_[ITEMTYPE::SKELETON]  = TERRAIN::SKELETON;
    impl_.itemmap_[ITEMTYPE::GHOUL]     = TERRAIN::GHOUL;
    impl_.itemmap_[ITEMTYPE::WIGHT]     = TERRAIN::WIGHT;
    impl_.itemmap_[ITEMTYPE::VAMPIRE]   = TERRAIN::VAMPIRE;
    impl_.itemmap_[ITEMTYPE::IMP]       = TERRAIN::IMP;
    impl_.itemmap_[ITEMTYPE::DEMON]     = TERRAIN::DEMON;
    impl_.itemmap_[ITEMTYPE::FIEND]     = TERRAIN::FIEND;
    impl_.itemmap_[ITEMTYPE::BUCKLER]   = TERRAIN::SHIELD;
    impl_.itemmap_[ITEMTYPE::SHIELD]    = TERRAIN::SHIELD;
    impl_.itemmap_[ITEMTYPE::SWORD]     = TERRAIN::WEAPON;
    impl_.itemmap_[ITEMTYPE::AXE]       = TERRAIN::WEAPON;
    impl_.itemmap_[ITEMTYPE::CHAINMAIL] = TERRAIN::ARMOR;
    impl_.itemmap_[ITEMTYPE::PLATEMAIL] = TERRAIN::ARMOR;
}

void View::message(const char *msg) {
    auto msgwin = impl_.message_.get();
    wclear(msgwin);
    mvwaddstr(msgwin, 0, 0, msg);
    wrefresh(msgwin);
}

void View::pause() {
    message("--press space to continue--");
    int c;
    do {
        c = getch();
    } while (c != ' ');
}

void View::refresh() {
    redrawwin(impl_.status_.get());
    redrawwin(impl_.message_.get());
    redrawwin(impl_.viewport_.get());
    doupdate();
}

void View::resize() {
    getmaxyx(stdscr, impl_.lines_, impl_.cols_);

    wbkgd(stdscr, ' ');

    auto viewport = impl_.viewport_.get();
    if (!viewport) {
        cbreak();
        noecho();
        nonl();
        impl_.viewport_.reset(subwin(stdscr, impl_.lines_, impl_.cols_, 0, 0));
    } else {
        wresize(viewport, impl_.lines_, impl_.cols_);
    }
    wbkgd(viewport, ' ' | COLOR_PAIR(4));

    auto message = impl_.message_.get();
    wresize(message, 1, impl_.cols_);
    wbkgd(message, ' ' | COLOR_PAIR(1));

    auto status = impl_.status_.get();
    wresize(status, 1, impl_.cols_);
    wbkgd(status, ' ' | COLOR_PAIR(3));
}

void View::shell() {
    def_prog_mode();
    endwin();
    std::fprintf(stderr, "Type 'exit' to return.\n");
    int returncode = std::system("/bin/sh");
    returncode += 0; // stops g++ warning for set but unused variable.
    reset_prog_mode();
}

// Private methods

View::ViewImpl::ViewImpl() : message_{nullptr}, status_{nullptr},
viewport_{nullptr}, commandkeys_{}, directionkeys_{}, itemmap_{}, tilemap_{},
lines_{0}, cols_{0} {
}

int View::ViewImpl::createMessageWin(WINDOW* win, int /* ncols */) {
    impl_.setMessageWin(win);

    return 0;
}

int View::ViewImpl::createStatusWin(WINDOW* win, int /* ncols */) {
    impl_.setStatusWin(win);

    return 0;
}

void View::ViewImpl::drawActors(World& world, int top, int left) {
    mvwaddch(viewport_.get(), world.playerRow() - top, world.playerCol() - left,
        tilemap_[TERRAIN::PLAYER] | COLOR_PAIR(5) | A_BOLD);
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
                t = itemmap_[item->type()];
                break;
                break;
        }
        mvwaddch(viewport_.get(), row - top, col - left,
            tilemap_[t] | COLOR_PAIR(7));
    });
}

void View::ViewImpl::drawMessage() {
    wnoutrefresh(message_.get());
}

void View::ViewImpl::drawStatus(Player& player) {
    auto status = status_.get();
    wclear(status);
    mvwprintw(status, 0, 1,
        "H: %2d    P: %2d    T: %2d    %s/%s/%s",
        player.health(), player.potions(), player.treasure(),
        player.weapon() != nullptr ? player.weapon()->name().c_str() : "no weapon",
        player.shield() != nullptr ? player.shield()->name().c_str() : "no shield",
        player.armor() != nullptr ? player.armor()->name().c_str() : "no armor"
    );

    wnoutrefresh(status);
}

void View::ViewImpl::drawViewport(World& world) {
    auto viewport = viewport_.get();
    int screenHeight, screenWidth;
    getmaxyx(viewport, screenHeight, screenWidth);

    wbkgd(viewport, tilemap_[TERRAIN::EMPTY]);

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
                display = tilemap_[t->terrain()];

                if (t->isBlock()) {
                    display |= COLOR_PAIR(2);
                }

            } else {
                continue;
            }

            mvwaddch(viewport, row, col, display);
        }
    }

    drawItems(world, top, left, screenHeight, screenWidth);

    drawActors(world, top, left);

    wnoutrefresh(viewport);
}

void View::ViewImpl::end_sig(int /* sig */) {
    View view;
    view.end();
}

void View::ViewImpl::setMessageWin(WINDOW* win) {

    message_.reset(win);
}

void View::ViewImpl::setStatusWin(WINDOW* win) {

    status_.reset(win);
}

