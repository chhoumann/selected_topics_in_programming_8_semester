#include "types.cpp"
#include "examples/seihr.cpp"
#include "graph_generator.cpp"
#include "stochastic_simulator.cpp"
#include "examples/circadian_oscillator.cpp"
#include "examples/simple.cpp"
#include "plot/plot.cpp"
#include <iostream>
#include <functional>
#include <memory>
#include <tuple>
#include "monitor/monitor.cpp"

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

void plot_circadian() {
    auto circadian_system = circadian_oscillator();
    auto trajectoryMonitor = SpeciesTrajectoryMonitor();
    auto ips_circadian = make_ips_counter(trajectoryMonitor);

    std::cout << "Simulating Circadian Rhythm..." << std::endl;
    auto s_circadian = Simulator(circadian_system, 100);
    s_circadian.simulate(ips_circadian);

    auto plot = plot_t("Trajectory of Circadian Rhythm", "Time, hours", "Count", 1920, 1080);
    for (const auto& [species, quantities] : trajectoryMonitor.getSpeciesQuantities()) {
        std::string speciesName = species.getName();

        if (speciesName == "C" || speciesName == "A" || speciesName == "R") {
            plot.lines(speciesName, trajectoryMonitor.getTimePoints(), quantities);
        }
    }

    plot.process();
    plot.save_to_png("circadian.png");
}

void plot_seihr() {
    auto seihr_system = seihr(10'000); // 589'755 for DK_NJ
    auto trajectoryMonitor = SpeciesTrajectoryMonitor();
    auto ips_seihr_species_monitor = make_ips_counter(trajectoryMonitor);

    std::cout << "Simulating SEIHR..." << std::endl;
    auto s_seihr = Simulator(seihr_system, 100);
    s_seihr.simulate(ips_seihr_species_monitor);

    auto plot = plot_t("Trajectory of SEIHR (N=10'000, rate=0.001)", "Time, days", "Count", 1920, 1080);
    for (const auto& [species, quantities] : trajectoryMonitor.getSpeciesQuantities()) {
        std::string speciesName = species.getName();

        if (speciesName == "H") {
            std::vector<double> transformedQuantities;
            std::transform(quantities.begin(), quantities.end(), std::back_inserter(transformedQuantities), [](double quantity) { return quantity * 1000; });

            plot.lines(speciesName + "*1000", trajectoryMonitor.getTimePoints(), transformedQuantities);
        } else {
            plot.lines(speciesName, trajectoryMonitor.getTimePoints(), quantities);
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
    auto s_simple = Simulator(simple_system, 100'000);
    s_simple.simulate(ips_species_monitor);

    auto plot_simple = plot_t("Trajectory of Simple (A=100, B=0, C=2)", "Time", "Count", 1920, 1080);
    for (const auto& [species, quantities] : trajectoryMonitor.getSpeciesQuantities()) {
        std::string speciesName = species.getName();

        plot_simple.lines(speciesName, trajectoryMonitor.getTimePoints(), quantities);
    }

    plot_simple.process();
    plot_simple.save_to_png("simple.png");
}

int main(int argc, char const *argv[])
{
    make_graphs();
    plot_simple();
    plot_circadian();
    plot_seihr();
}
