#include "types.cpp"
#include "seihr.cpp"
#include "graph_generator.cpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    auto v = seihr(100'000);
    auto const& reactions = v.getReactions();

    std::string filename = "seihr.png";
    generate_graph(reactions, filename);

    std::cout << "pls" << std::endl;

    return 0;
}
