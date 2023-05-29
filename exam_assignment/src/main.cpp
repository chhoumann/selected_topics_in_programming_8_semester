#include "types.cpp"
#include "examples/seihr.cpp"
#include "graph_generator.cpp"
#include "stochastic_simulator.cpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    auto v = seihr(100'000);
    //auto const& reactions = v.getReactions();

    //std::string filename = "seihr.png";
    //generate_graph(reactions, filename);

    auto s = Simulator(v, 1000000);
    s.simulate();

    return 0;
}
