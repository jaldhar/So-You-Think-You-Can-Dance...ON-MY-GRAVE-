// Item -- An item in S.Y.T.Y.C.D...O.M.G? (interface)
//

#ifndef ITEM_H
#define ITEM_H 1

#include <string>

class Item {
public:
    Item();
    Item(std::string name, std::string article);
    virtual ~Item() { };
    virtual Item* clone();
    std::string name();
    std::string article();
protected:
    std::string _name;
    std::string _article;
};

#endif
