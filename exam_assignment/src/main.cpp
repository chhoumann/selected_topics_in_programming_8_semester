#include "types.cpp"
#include "examples/seihr.cpp"
#include "graph_generator.cpp"
#include "stochastic_simulator.cpp"
#include "examples/circadian_oscillator.cpp"
#include "plot/plot.cpp"
#include <iostream>

int main(int argc, char const *argv[])
{
    //auto v = seihr(100'000);
    auto v = circadian_oscillator();

    //auto const& reactions = v.getReactions();

    //std::string filename = "seihr.png";
    //generate_graph(reactions, filename);

    std::vector<double> timePoints;
    std::map<Species, std::vector<double>> speciesQuantities;

    auto monitor = [&timePoints, &speciesQuantities](const System& system, double t) {
        const auto& species = system.getSpecies();

        // Store the time point
        timePoints.push_back(t);

        // Store the quantity of each species
        for (const auto& [speciesName, quantity] : species) {
            speciesQuantities[speciesName].push_back(quantity);
        }
    };


    auto s = Simulator(v, 100);
    s.simulate(monitor);

    plot_t plot = plot_t("Trajectory of Circadian Rhythm", 800, 800);
    for (const auto& [species, quantities] : speciesQuantities) {
        std::string speciesName = species.getName();

        if (speciesName == "C" || speciesName == "A" || speciesName == "R") {
            plot.lines(speciesName, timePoints, quantities);
        }
    }
    plot.process();

    return 0;
}
