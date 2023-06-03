#include "make_graphs.h"

// Solves requirement 2b: Pretty-print the reaction network in a network graph.
void make_graphs() {
    auto seihr_system = seihr(100'000);
    auto const& reactions_seihr = seihr_system.getReactions();

    std::string filename_seihr = "seihr_graph.png";
    generate_graph(reactions_seihr, filename_seihr);

    auto circadian_system = circadian_oscillator();
    auto const& reactions_circadian = circadian_system.getReactions();

    std::string filename_circadian = "circadian_graph.png";
    generate_graph(reactions_circadian, filename_circadian);

    auto simple_system = simple();
    auto const& reactions_simple = simple_system.getReactions();

    std::string filename_simple = "simple_graph.png";
    generate_graph(reactions_simple, filename_simple);
}
