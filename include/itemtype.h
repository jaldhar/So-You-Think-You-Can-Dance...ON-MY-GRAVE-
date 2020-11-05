// ITEMTYPE -- Identifiers for each kind of item in S.Y.T.Y.C.D...O.M.G? (enum)

#ifndef ITEMTYPE_H
#define ITEMTYPE_H 1

#include <cstdint>

enum class ITEMTYPE : std::uint8_t { NOTHING = 0, DOOR, POTION, TREASURE,
ZOMBIE, SKELETON, GHOUL, WIGHT, VAMPIRE, IMP, DEMON, FIEND,
BUCKLER, SHIELD, SWORD, AXE, CHAINMAIL, PLATEMAIL };

#endif
