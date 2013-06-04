// Room -- A room in S.Y.T.Y.C.D...O.M.G? (implementation)

#include <cstdlib>
using namespace std;

#include "room.h"

struct Room::RoomImpl {
    RoomImpl(int yoffset, int xoffset, int height, int width);
    ~RoomImpl()=default;

    int _top;
    int _left;
    int _height;
    int _width;
    int _centerX;
    int _centerY;
};

Room::Room(int yoffset, int xoffset, int height, int width) :
    _impl { new Room::RoomImpl(yoffset, xoffset, height, width) } {
}

Room::~Room()=default;

int Room::top() const {
    return _impl->_top;
}

int Room::left() const {
    return _impl->_left;
}

int Room::height() const {
    return _impl->_height;
}

int Room::width() const {
    return _impl->_width;
}

int Room::centerX() const {
    return _impl->_centerX;
}

int Room::centerY() const {
    return _impl->_centerY;
}

Room::RoomImpl::RoomImpl(int yoffset, int xoffset, int height, int width) {
    // Randomly set the room dimensions.  Align rooms on even numbered tiles
    // only.
    _top =  rand() % (height / 4) + 2;
    if (_top % 2) {
        _top += 1;
    }
    int bottom = height - 3 - (rand() % (height / 4));
    if (bottom % 2) {
        bottom -= 1;
    }
    _height = bottom - _top + 1;

    _left =  rand() % (width / 4) + 2;
    if (_left % 2) {
        _left += 1;
    }
    int right = width - 3 - (rand() % (width / 4));
    if (right % 2) {
        right -= 1;
    }
    _width = right - _left + 1;

    _centerX = _left + (rand() % _width);
    if (_centerX % 2) {
        _centerX -= 1;
    }
    _centerY = _top + (rand() % _height);
    if (_centerY % 2) {
        _centerY -= 1;
    }

    // Adjust the locations according to their positions on the map
    _centerY += yoffset;
    _centerX += xoffset;
    _top += yoffset;
    _left += xoffset;
}

