#include "types.cpp"
#include "circadian_oscillator.cpp"

// Use operator overloading to support the reaction rule typesetting directly in C++ code.
std::ostream& operator<<(std::ostream& os, const Reaction& reaction) {
    os << reaction.reactants
    return os;
}

int main(int argc, char const *argv[])
{
    auto v = circadian_oscillator();

    // auto sim = StochasticSimulator(v);

    return 0;
}
