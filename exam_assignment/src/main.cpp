#include "types.cpp"
#include "examples/seihr.cpp"
#include "graph_generator.cpp"
#include "stochastic_simulator.cpp"
#include "examples/circadian_oscillator.cpp"
#include "plot/plot.cpp"
#include <iostream>
#include <functional>

void make_graphs() {
    auto seihr_system = seihr(100'000);
    auto const& reactions_seihr = seihr_system.getReactions();

    std::string filename_seihr = "seihr.png";
    generate_graph(reactions_seihr, filename_seihr);

    auto circadian_system = circadian_oscillator();
    auto const& reactions_circadian = circadian_system.getReactions();

    std::string filename_circadian = "circadian.png";
    generate_graph(reactions_circadian, filename_circadian);
}

void plot_circadian() {
    auto circadian_system = circadian_oscillator();

    std::vector<double> timePoints_circadian;
    std::map<Species, std::vector<double>> speciesQuantities_circadian;

    auto monitor_circadian = [&timePoints_circadian, &speciesQuantities_circadian](const System& system, double t) {
        std::cout << "Circadian: " << "t = " << t << std::endl;
        const auto& species = system.getSpecies();

        // Store the time point
        timePoints_circadian.push_back(t);

        // Store the quantity of each species
        for (const auto& [speciesName, quantity] : species) {
            speciesQuantities_circadian[speciesName].push_back(quantity);
        }
    };

    std::cout << "Simulating Circadian Rhythm..." << std::endl;
    auto s_circadian = Simulator(circadian_system, 100);
    s_circadian.simulate(monitor_circadian);

    plot_t plot_circadian = plot_t("Trajectory of Circadian Rhythm", 800, 800);
    for (const auto& [species, quantities] : speciesQuantities_circadian) {
        std::string speciesName = species.getName();

        if (speciesName == "C" || speciesName == "A" || speciesName == "R") {
            plot_circadian.lines(speciesName, timePoints_circadian, quantities);
        }
    }

    plot_circadian.process();
}

template <typename Func>
auto make_ips_counter(Func func) {
    return [func](auto&&... args) {
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

void plot_seihr() {
    auto seihr_system = seihr(10'000); // 589'755 for DK_NJ

    std::vector<double> timePoints_seihr;
    std::map<Species, std::vector<double>> speciesQuantities_seihr;

    auto monitor_seihr = [&timePoints_seihr, &speciesQuantities_seihr](const System& system, double t) {
        const auto& species = system.getSpecies();

        // Store the time point
        timePoints_seihr.push_back(t);

        // Store the quantity of each species
        for (const auto& [speciesName, quantity] : species) {
            speciesQuantities_seihr[speciesName].push_back(quantity);
        }
    };

    auto ips_seihr_species_monitor = make_ips_counter(monitor_seihr);

    std::cout << "Simulating SEIHR..." << std::endl;
    auto s_seihr = Simulator(seihr_system, 100);
    s_seihr.simulate(ips_seihr_species_monitor);

    plot_t plot_seihr = plot_t("Trajectory of SEIHR", 800, 800);
    for (const auto& [species, quantities] : speciesQuantities_seihr) {
        std::string speciesName = species.getName();

        if (speciesName == "H") {
            std::vector<double> transformedQuantities;
            std::transform(quantities.begin(), quantities.end(), std::back_inserter(transformedQuantities), [](double quantity) { return quantity * 1000; });
            plot_seihr.lines(speciesName + "*1000", timePoints_seihr, transformedQuantities);
        } else {
            plot_seihr.lines(speciesName, timePoints_seihr, quantities);
        }
    }

    plot_seihr.process();
}

int main(int argc, char const *argv[])
{
    plot_seihr();
}
