// Model -- Abstract base class for S.Y.T.Y.C.D...O.M.G? Data (interface)
//

#ifndef MODEL_H
#define MODEL_H 1

class Player;
class Tile;

struct Model {
    virtual void          create() = 0;
    virtual int           height() = 0;
    virtual int           width() = 0;
    virtual int           sectorHeight() = 0;
    virtual int           sectorWidth() = 0;
    virtual Player&       player() = 0;
    virtual int           playerRow() = 0;
    virtual void          setPlayerRow(int row) = 0;
    virtual int           playerCol() = 0;
    virtual void          setPlayerCol(int col) = 0;
    virtual Tile&         tileAt(int row, int col) = 0;
};

#endif
