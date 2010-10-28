// Item -- An Item in S.Y.T.Y.C.D...O.M.G? (implementation)
//

using namespace std;

#include "item.h"

Item::Item() {
    _name = "item";
    _article = "an";
}

Item::Item(string name, string article) {
    _name = name;
    _article = article;
}

string Item::name(){
    return _name;
}

string Item::article(){
    return _article;
}

Item* Item::clone(){
    return new Item(*this);
}
