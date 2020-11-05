// Door -- A door in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef DOOR_H
#define DOOR_H 1

#include <memory>
#include "item.h"

class Door : public Item {
public:
    Door();
    virtual ~Door();
    bool horizontal() const;
    void setHorizontal(bool horizontal);
    bool open() const;
    void setOpen(bool open);

private:
    struct DoorImpl;
    std::unique_ptr<DoorImpl> impl_;
};

#endif
