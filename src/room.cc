// Room -- A room in S.Y.T.Y.C.D...O.M.G? (implementation)

#include <cstdlib>

#include "room.h"

struct Room::RoomImpl {
    RoomImpl(int yoffset, int xoffset, int height, int width);
    ~RoomImpl()=default;

    int top_;
    int left_;
    int height_;
    int width_;
    int centerX_;
    int centerY_;
};

Room::Room(int yoffset, int xoffset, int height, int width) :
    impl_ { new Room::RoomImpl(yoffset, xoffset, height, width) } {
}

Room::~Room()=default;

int Room::top() const {
    return impl_->top_;
}

int Room::left() const {
    return impl_->left_;
}

int Room::height() const {
    return impl_->height_;
}

int Room::width() const {
    return impl_->width_;
}

int Room::centerX() const {
    return impl_->centerX_;
}

int Room::centerY() const {
    return impl_->centerY_;
}

Room::RoomImpl::RoomImpl(int yoffset, int xoffset, int height, int width) :
top_{0}, left_{0}, height_{0}, width_{0}, centerX_{0}, centerY_{0} {
    // Randomly set the room dimensions.  Align rooms on even numbered tiles
    // only.
    top_ =  std::rand() % (height / 4) + 2;
    if (top_ % 2) {
        top_ += 1;
    }
    int bottom = height - 3 - (std::rand() % (height / 4));
    if (bottom % 2) {
        bottom -= 1;
    }
    height_ = bottom - top_ + 1;

    left_ =  std::rand() % (width / 4) + 2;
    if (left_ % 2) {
        left_ += 1;
    }
    int right = width - 3 - (std::rand() % (width / 4));
    if (right % 2) {
        right -= 1;
    }
    width_ = right - left_ + 1;

    centerX_ = left_ + (std::rand() % width_);
    if (centerX_ % 2) {
        centerX_ -= 1;
    }
    centerY_ = top_ + (std::rand() % height_);
    if (centerY_ % 2) {
        centerY_ -= 1;
    }

    // Adjust the locations according to their positions on the map
    centerY_ += yoffset;
    centerX_ += xoffset;
    top_ += yoffset;
    left_ += xoffset;
}

