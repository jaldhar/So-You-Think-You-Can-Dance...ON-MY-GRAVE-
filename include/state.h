// STATE -- Represents states in the S.Y.T.Y.C.D...O.M.G? state machine (enum)

#ifndef STATE_H
#define STATE_H 1

#include <cstdint>

enum class STATE : std::uint8_t { ERROR = 0, COMMAND, FIGHTING, MOVING, QUIT };

#endif
