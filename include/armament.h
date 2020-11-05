// Armament -- base class for a thing that can aid combat in S.Y.T.Y.C.D...O.M.G?
// (interface)
//

#ifndef ARMAMENT_H
#define ARMAMENT_H 1

#include <memory>

class Armament {
public:
    Armament();
    Armament(int defenseBonus, int offenseBonus);
    virtual ~Armament();
    int  defenseBonus() const;
    void setDefenseBonus(int defenseBonus);
    int  offenseBonus() const;
    void setOffenseBonus(int offenseBonus);

private:
    struct ArmamentImpl;
    std::unique_ptr<ArmamentImpl> impl_;
};

#endif
