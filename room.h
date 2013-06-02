// Room -- A room in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef ROOM_H
#define ROOM_H 1

#include <memory>

class Room {
public:
    Room(int yoffset, int xoffset, int height, int width);
    ~Room();
    void fill();
    int  top() const;
    int  left() const;
    int  height() const;
    int  width() const;
    int  centerX() const;
    int  centerY() const;

private:
    struct RoomImpl;
    std::unique_ptr<RoomImpl> _impl;
};

#endif
