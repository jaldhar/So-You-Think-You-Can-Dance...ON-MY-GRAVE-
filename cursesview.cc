// CursesView -- S.Y.T.Y.C.D...O.M.G? display using curses (implementation)
//

#define __USE_POSIX
#include <cctype>
#include <clocale>
#include <cstdlib>
#include <cstring>
#include <csignal>
using namespace std;

#include "sytycdomg.h"
#include "cursesview.h"
#include "monster.h"
#include "potion.h"
#include "tile.h"
#include "treasure.h"
#include "player.h"
#include "world.h"

extern Game* a;
extern CursesView*     v;
extern World*          w;

CursesView* CursesView::_instance = 0;

CursesView* CursesView::instance(){
    if(_instance == 0)
        _instance = new CursesView();

    return _instance;
}

static void end_sig(int /* sig */) {
    v->end();
}

static int createStatusWin(WINDOW *win, int /* ncols */) {
    v->setStatusWin(win);

    return 0;
}

static int createTitleWin(WINDOW *win, int /* ncols */) {
    v->setTitleWin(win);

    return 0;
}

CursesView::CursesView() {
    signal (SIGINT, end_sig);
    signal (SIGSEGV, end_sig);

    viewport = 0;
    lines = 0;
    cols  = 0;
    top   = 0;
    left  = 0;
}

CursesView::~CursesView() {
    if(_instance != 0)
        delete _instance;
}

void CursesView::draw() {
    curs_set(0);
    drawViewport();
    drawStatus();
    drawTitle();
    refresh();
}

void CursesView::drawStatus() {
    Player p = w->player();

    wclear(status);
    mvwprintw(status, 0, 1,
        "H: %2d    P: %2d    T: %2d    %s/%s/%s",
        p.health(), p.potions(), p.treasure(),
        p.weapon() ? p.weapon()->name().c_str() : "no weapon",
        p.shield() ? p.shield()->name().c_str() : "no shield",
        p.armor() ? p.armor()->name().c_str() : "no armor"
    );
    wnoutrefresh(status);
}

void CursesView::drawTitle() {
    wclear(title);
    const char * const titleText = "So You Think You Can Dance...ON MY GRAVE?";
    const int len = ::strlen(titleText);
    mvwaddstr(title, 0, (cols - len)/2, titleText);
    wnoutrefresh(title);
}

void CursesView::drawViewport() {
    int height,width;

    wclear(viewport);
    getmaxyx(viewport, height, width);

    int top = w->playerRow() - (height / 2);
    if (top < 0) {
        top = 0;
    }
    int left = w->playerCol() - (width / 2);
    if (left < 0) {
        left = 0;
    }
    for (int row = 0; row < height; row++) {
        wmove(viewport, row, 0);
        int maprow = row + top;
        if (maprow > w->height() - 1) {
            continue;
        }
        for (int col = 0; col < width; col++) {
            int mapcol = col + left;
            if (mapcol > w->width() - 1) {
                continue;
            }
            chtype display;
            if (maprow == w->playerRow() && mapcol == w->playerCol()) {
                display = '@' | COLOR_PAIR(5);
            }
            else if (maprow > w->playerRow() - 3 &&
            maprow < w->playerRow()+3 &&
            mapcol > w ->playerCol() - 3 &&
            mapcol < w->playerCol() + 3) {
                if(isVisible(maprow, mapcol)) {
                    Tile t = w->tileAt(maprow, mapcol);
                    switch (t.terrain()) {
                        case FLOOR: display = ' ';
                        break;
                      case WALL:  display = ACS_BOARD;
                        break;
                        case DOOR:  display = 'x';
                        break;
                        case EMPTY:
                        default:
                                    display = ACS_CKBOARD;
                        break;
                    }
                    Item *contents = t.contents();
                    if ( contents != 0 ) {
                        if(dynamic_cast<Monster*>(contents)) {
                            display =
                                dynamic_cast<Monster*>(contents)->symbol() |
                                COLOR_PAIR(6);
                        }
                        else if(dynamic_cast<Treasure*>(contents)) {
                            display = 'T' | COLOR_PAIR(6);
                        }
                        else if(dynamic_cast<Potion*>(contents)) {
                            display = 'P' | COLOR_PAIR(6);
                        }
                        else {
                            display = 'I' | COLOR_PAIR(6);
                        }
                    }
                }
                else {
                    display = ACS_CKBOARD;
                }
            }
            else {
                display = ACS_CKBOARD;
            }
            waddch(viewport, display);
        }
    }
    wnoutrefresh(viewport);
}

void CursesView::end() {
    curs_set(1);
    endwin();
    clear();
    exit(EXIT_SUCCESS);
}

STATE CursesView::handleInput() {
    int c = getch();

    KEYMAP::iterator it = keybindings.begin();
    if ((it = keybindings.find(c)) != keybindings.end())
        return (a->*it->second)();

    beep();
    return ERROR;
}

void CursesView::init() {
    setlocale(LC_ALL, "POSIX");
    nonl();
    cbreak();
    noecho();

    keybindings[0x12] /* CTRL-R */ = &Game::refresh;
    keybindings['Q']               = &Game::quit;
    keybindings['!']               = &Game::shell;
    keybindings['h']               = &Game::move_left;
    keybindings[KEY_LEFT]          = &Game::move_left;
    keybindings['j']               = &Game::move_down;
    keybindings[KEY_DOWN]          = &Game::move_down;
    keybindings['k']               = &Game::move_up;
    keybindings[KEY_UP]            = &Game::move_up;
    keybindings['l']               = &Game::move_right;
    keybindings[KEY_RIGHT]         = &Game::move_right;
    keybindings['y']               = &Game::move_upleft;
    keybindings['u']               = &Game::move_upright;
    keybindings['b']               = &Game::move_downleft;
    keybindings['n']               = &Game::move_downright;
    keybindings['q']               = &Game::quaff;
    keybindings['t']               = &Game::take;
    keybindings[KEY_RESIZE]        = &Game::resize;

    ripoffline(1, createTitleWin);
    ripoffline(-1, createStatusWin);
    initscr();
    keypad(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_BLACK, COLOR_YELLOW);
        init_pair(3, COLOR_BLACK, COLOR_CYAN);
        init_pair(4, COLOR_BLACK, COLOR_BLACK);
        init_pair(5, COLOR_BLUE,  COLOR_WHITE);
        init_pair(6, COLOR_RED,   COLOR_WHITE);
    }

    resize();
}

void CursesView::pause() {
    mvwaddstr(status, 0, 1, "--press a key to continue--");
    wclrtoeol(status);
    wrefresh(status);
    getch();
}

void CursesView::refresh() {
    wrefresh(status);
    wrefresh(title);
    wrefresh(viewport);
}

void CursesView::resize() {
    getmaxyx(stdscr, lines, cols);

    wbkgd(stdscr, ' ');

    if (!viewport) {
        viewport = subwin(stdscr, lines, cols, 0, 0);
    }
    else {
        wresize(viewport, lines, cols);
    }
    wbkgd(viewport, ' ' | COLOR_PAIR(1));

    wresize(title, 1, cols);
    wbkgd(title, ' ' | COLOR_PAIR(2));

    wresize(status, 1, cols);
    wbkgd(status, ' ' | COLOR_PAIR(3));

    draw();
}

void CursesView::setStatusWin(WINDOW *&win) {

    status = win;
}

void CursesView::setTitleWin(WINDOW *&win) {

    title = win;
}

void CursesView::shell() {
    def_prog_mode();
    endwin();
    int returncode;
    fprintf(stderr, "Type 'exit' to return.\n");
    returncode = system("/bin/sh");
    reset_prog_mode();
}

bool CursesView::isVisible(int y, int x) {
    if (y < w->playerRow()) {
        if (x < w->playerCol()) {
            for (int i = y + 1 , j = x + 1;
            i < w->playerRow() &&  j < w->playerCol(); i++, j++) {
                if (w->tileAt(i,j).terrain() == WALL) {
                    return false;
                }
            }
        }
        else if (x > w->playerCol()) {
            for (int i = y + 1, j = x - 1;
            i < w->playerRow() &&  j > w->playerCol(); i++, j--) {
                if (w->tileAt(i,j).terrain() == WALL) {
                    return false;
                }
            }
        }
        else {
            for (int i = y + 1; i < w->playerRow(); i++) {
                if (w->tileAt(i,x).terrain() == WALL) {
                    return false;
                }
            }
        }
    }
    else if (y > w->playerRow()) {
        if(x < w->playerCol()) {
            for (int i = y - 1, j = x + 1;
            i > w->playerRow() &&  j < w->playerCol(); i--, j++) {
                if (w->tileAt(i,j).terrain() == WALL) {
                    return false;
                }
            }
        }
        else if (x > w->playerCol()) {
            for (int i = y - 1, j = x - 1;
            i > w->playerRow() &&  j > w->playerCol();
            i--, j--) {
                if (w->tileAt(i,j).terrain() == WALL) {
                    return false;
                }
            }
        }
        if (x == w->playerCol()) {
            for (int i = y - 1; i > w->playerRow(); i--) {
                if (w->tileAt(i,x).terrain() == WALL) {
                    return false;
                }
            }
        }
    }
    else {
        if (x < w->playerCol()) {
            for (int j = x + 1; j < w->playerCol(); j++) {
                if (w->tileAt(y,j).terrain() == WALL) {
                    return false;
                }
            }
        }
        else if (y == w->playerRow() && x > w->playerCol()) {
            for (int j = x - 1; j < w->playerCol(); j++) {
                if (w->tileAt(y,j).terrain() == WALL) {
                    return false;
                }
            }
        }
    }

    return true;
}
