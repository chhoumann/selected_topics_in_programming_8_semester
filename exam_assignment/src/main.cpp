#include "types.cpp"
#include "circadian_oscillator.cpp"
#include "graph_generator.cpp"

int main(int argc, char const *argv[])
{
    auto v = circadian_oscillator();
    auto const& reactions = v.getReactions();

    std::string filename = "circadian_oscillator.png";
    generate_graph(reactions, filename);

    return 0;
}
