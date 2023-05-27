#include "types.cpp"
#include "circadian_oscillator.cpp"
#include "graph_generator.cpp"

int main(int argc, char const *argv[])
{
    auto v = circadian_oscillator();
    auto reactions = v.getReactions();

    generate_graph(reactions, "circadian_oscillator.png");

    // auto sim = StochasticSimulator(v);

    return 0;
}
