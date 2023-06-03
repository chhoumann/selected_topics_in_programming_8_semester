#include <iostream>
#include <tuple>

#include "types.h"
#include "graph_generator.h"
#include "stochastic_simulator.h"
#include "plot/plot.hpp"
#include "exercises/make_graphs.h"
#include "exercises/benchmark.h"
#include "exercises/peak_avg_seihr.h"

#include "examples/examples.h"
#include "monitor/species_trajectory_monitor.h"

// Utility function to count iterations per second by wrapping Monitors. For my own purposes.
template <typename Func>
auto make_ips_counter(Func& func) {
    return [&func](auto&&... args) {
        static auto lastSecond = std::chrono::steady_clock::now();
        static int iterations = 0;

        func(std::forward<decltype(args)>(args)...);
        iterations++;

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastSecond).count() >= 1) {
            std::cout << "Iterations per second: " << iterations / 1000 << "k" << '\n';
            iterations = 0;
            lastSecond = now;
        }
    };
}

void plot_circadian() {
    auto circadian_system = circadian_oscillator();
    auto trajectoryMonitor = SpeciesTrajectoryMonitor();
    auto ips_circadian = make_ips_counter(trajectoryMonitor);

    std::cout << "Simulating Circadian Rhythm..." << std::endl;
    auto s_circadian = Simulator(circadian_system, 100);
    s_circadian.simulate(ips_circadian);

    auto plot = plot_t("Trajectory of Circadian Rhythm", "Time, hours", "Count", 1920, 1080);
    for (const auto& [species, quantities] : *trajectoryMonitor.speciesQuantities) {
        std::string speciesName = species.getName();

        if (speciesName == "C" || speciesName == "A" || speciesName == "R") {
            plot.lines(speciesName, *trajectoryMonitor.timePoints, quantities);
        }
    }

    plot.process();
    plot.save_to_png("circadian.png");
}

void plot_seihr() {
    auto seihr_system = seihr(10000);
    auto trajectoryMonitor = SpeciesTrajectoryMonitor();
    auto ips_seihr_species_monitor = make_ips_counter(trajectoryMonitor);

    std::cout << "Simulating SEIHR..." << std::endl;
    auto s_seihr = Simulator(seihr_system, 100);
    s_seihr.simulate(ips_seihr_species_monitor);

    auto plot = plot_t("Trajectory of SEIHR (N=10000)", "Time, days", "Count", 1920, 1080);
    for (const auto& [species, quantities] : *trajectoryMonitor.speciesQuantities) {
        std::string speciesName = species.getName();

        // Multiply H by 1000 to make it more visible in the graph
        if (speciesName == "H") {
            std::vector<double> transformedQuantities;
            std::transform(quantities.begin(), quantities.end(), std::back_inserter(transformedQuantities), [](double quantity) { return quantity * 1000; });

            plot.lines(speciesName + "*1000", *trajectoryMonitor.timePoints, transformedQuantities);
        } else {
            plot.lines(speciesName, *trajectoryMonitor.timePoints, quantities);
        }
    }

    plot.process();
    plot.save_to_png("seihr.png");
}

void plot_simple() {
    auto simple_system = simple();
    auto trajectoryMonitor = SpeciesTrajectoryMonitor();
    auto ips_species_monitor = make_ips_counter(trajectoryMonitor);

    std::cout << "Simulating Simple..." << std::endl;
    auto s_simple = Simulator(simple_system, 100000);
    s_simple.simulate(ips_species_monitor);

    auto plot_simple = plot_t("Trajectory of Simple (A=100, B=0, C=2)", "Time", "Count", 1920, 1080);
    for (const auto& [species, quantities] : *trajectoryMonitor.speciesQuantities) {
        std::string speciesName = species.getName();

        plot_simple.lines(speciesName, *trajectoryMonitor.timePoints, quantities);
    }

    plot_simple.process();
    plot_simple.save_to_png("simple.png");
}

int main(int argc, char const *argv[])
{
    // Solves requirement 2a: Pretty-print the reaction network in human readable format
    auto seihr_sys = seihr(100000);
    auto const& seihr_reactions = seihr_sys.getReactions();

    std::cout << "SEIHR reactions:\n";
    for (const auto& reaction : seihr_reactions) {
        std::cout << reaction << '\n';
    }

    auto circadian_sys = circadian_oscillator();
    auto const& circadian_reactions = circadian_sys.getReactions();

    std::cout << "Circadian reactions:\n";
    for (const auto& reaction : circadian_reactions) {
        std::cout << reaction << '\n';
    }

    auto simple_sys = simple();
    auto const& simple_reactions = simple_sys.getReactions();

    std::cout << "Simple reactions:\n";
    for (const auto& reaction : simple_reactions) {
        std::cout << reaction << '\n';
    }

    // Solves requirement 2b: Pretty-print the reaction network in a network graph
    make_graphs();

    // Solves requirement 5: Demonstrate the application of the library on the three examples (shown in Fig. 1, 2, 3).
    // Solves requirement 6: Display simulation trajectories of how the amounts change. External tools/libraries can be used to visualize.
    plot_simple();
    plot_circadian();
    plot_seihr();

    // Solution to second part of requirement 7: Use it to estimate
    // the peak of hospitalized agents in Covid-19 example without storing trajectory data for NNJ and NDK.
    // Solution to the second part of requirement 8: Estimate the likely (mean) value of the hospitalized peak over 20 simulations.
    // Except I did it over 100 simulations.
    const size_t N_NJ = 589755;
    const size_t N_DK = 5882763;

    std::cout << "SEIHR peak and avg hospitalized agents for North Jutland population size" << std::endl;
    calculate_peak_and_avg_seihr(100, 12, N_NJ);

    std::cout << "SEIHR peak and avg hospitalized agents for Denmark population size" << std::endl;
    calculate_peak_and_avg_seihr(100, 12, N_DK);

    // Solves requirement 10: Benchmark and compare the stochastic simulation performance (e.g. the time it takes to compute 20 simulations
    // a single core, multiple cores, or improved implementation). Make your conclusions.
    do_benchmarks();

    return 0;
}
