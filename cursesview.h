// CursesView -- S.Y.T.Y.C.D...O.M.G? display using curses (interface)
//

#ifndef CURSESVIEW_H
#define CURSESVIEW_H 1

#include <map>
#include <curses.h>
#include "game.h"
#include "view.h"

class CursesView : virtual public View {
public:
    void  draw();
    void  end();
    STATE handleInput();
    void  init();
    void  pause();
    void  resize();
    void  refresh();
    void  shell();

public:
    static CursesView* instance();
    ~CursesView();
    void  setStatusWin(WINDOW *&win);
    void  setTitleWin(WINDOW *&win);

protected:
    CursesView();
    void  drawStatus();
    void  drawTitle();
    void  drawViewport();
    bool  isVisible(int y, int x);
    typedef STATE(Game::*command_ptr)(void);
    typedef std::map<int, command_ptr> KEYMAP;
    WINDOW*            status;
    WINDOW*            title;
    WINDOW*            viewport;
    KEYMAP             keybindings;
    std::string        _prompt;
    int                lines;
    int                cols;
    int                top;
    int                left;
    static CursesView*  _instance;
};

#endif
