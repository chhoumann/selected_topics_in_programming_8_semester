#include "types.cpp"
#include "circadian_oscillator.cpp"
#include <iostream>

// Use operator overloading to support the reaction rule typesetting directly in C++ code.
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

int main(int argc, char const *argv[])
{
    auto v = circadian_oscillator();

    // Print all reactions in v
    for (const auto& reaction : v.getReactions()) {
        std::cout << reaction << std::endl;
    }

    // auto sim = StochasticSimulator(v);

    return 0;
}
