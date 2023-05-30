#include "types.cpp"
#include "examples/seihr.cpp"
#include "graph_generator.cpp"
#include "stochastic_simulator.cpp"
#include "examples/circadian_oscillator.cpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    //auto v = seihr(100'000);
    auto v = circadian_oscillator();

    //auto const& reactions = v.getReactions();

    //std::string filename = "seihr.png";
    //generate_graph(reactions, filename);

    auto s = Simulator(v, 100);
    s.simulate();

    return 0;
}
