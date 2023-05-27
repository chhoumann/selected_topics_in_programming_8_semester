#include <iostream>
#include "types.cpp"

std::ostream& operator<<(std::ostream& os, const Reaction& reaction) {
    os << reaction.reactants[0].getName();
    for (int i = 1; i < reaction.reactants.size(); i++) {
        os << " + " << reaction.reactants[i].getName();
    }

    os << " → " << reaction.products[0].getName();
    for (int i = 1; i < reaction.products.size(); i++) {
        os << " + " << reaction.products[i].getName();
    }

    os << " (rate: " << reaction.delay << ")";

    return os;
}