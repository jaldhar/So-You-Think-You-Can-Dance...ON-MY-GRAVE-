// DIRECTION -- Represents directions in S.Y.T.Y.C.D...O.M.G? (enum)

#ifndef DIRECTION_H
#define DIRECTION_H 1

#include <cstdint>

enum class DIRECTION : std::uint8_t { NO_DIRECTION = 0, NORTH, EAST, WEST,
    SOUTH, NORTHWEST, NORTHEAST, SOUTHWEST, SOUTHEAST };

#endif
