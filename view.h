// View -- S.Y.T.Y.C.D...O.M.G? display (interface)
//

#ifndef VIEW_H
#define VIEW_H 1

#include <string>
#include "direction.h"
#include "state.h"

class View {
public:
    View();
    ~View();
    void  alert();
    void  draw();
    void  end();
    STATE handleTopLevelInput();
    DIRECTION handleDirectionInput();
    void  init();
    void  message(const char *msg);
    void  pause();
    void  refresh();
    void  resize();
    void  shell();

private:
    struct ViewImpl;
    static ViewImpl _impl;
};

#endif