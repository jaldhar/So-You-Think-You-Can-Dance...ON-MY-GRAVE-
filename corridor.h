// Corridor -- A corridor in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef CORRIDOR_H
#define CORRIDOR_H 1

class Corridor {
public:
    Corridor(int y1, int x1, int y2, int x2);
    void wallfill_north(int y, int x);
    void wallfill_south(int y, int x);
    void wallfill_west(int y, int x);
    void wallfill_east(int y, int x);
private:
    int _y1, _x1, _y2, _x2;
};

#endif
