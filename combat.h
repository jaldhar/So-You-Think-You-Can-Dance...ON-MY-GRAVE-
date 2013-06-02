// Combat -- S.Y.T.Y.C.D...O.M.G? combat system (interface)
//

#ifndef COMBAT_H
#define COMBAT_H 1

#include <memory>

class Combat {
public:
    Combat();
    Combat(int health, int offense, int defense);
    virtual ~Combat();
    int  attack();
    int  defend();
    int  defense() const;
    void setDefense(int defense);
    int  health() const;
    void setHealth(int health);
    int  offense() const;
    void setOffense(int offense);

private:
    struct CombatImpl;
    std::unique_ptr<CombatImpl> _impl;
};

#endif
