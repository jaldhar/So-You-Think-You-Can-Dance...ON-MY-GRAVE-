// View -- Abstract base class for S.Y.T.Y.C.D...O.M.G? displays
//

#ifndef VIEW_H
#define VIEW_H 1

#include <string>
#include "state.h"

struct View {
    virtual void  draw() = 0;
    virtual void  end() = 0;
    virtual STATE handleInput() = 0;
    virtual void  init() = 0;
    virtual void  pause() = 0;
    virtual void  resize() = 0;
    virtual void  refresh() = 0;
    virtual void  shell() = 0;
};

#endif
