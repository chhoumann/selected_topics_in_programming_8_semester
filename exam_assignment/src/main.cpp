#include <iostream>
#include <tuple>

#include "types.h"
#include "graph_generator.h"
#include "stochastic_simulator.h"
#include "monitor/monitor.h"
#include "plot/plot.hpp"
#include "thread_pool.cpp"
#include "exercises/make_graphs.h"
#include "exercises/benchmark.cpp"

#include "examples/examples.h"

#include "monitor/monitors.cpp"

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
    auto seihr_system = seihr(10'000);
    auto trajectoryMonitor = SpeciesTrajectoryMonitor();
    auto ips_seihr_species_monitor = make_ips_counter(trajectoryMonitor);

    std::cout << "Simulating SEIHR..." << std::endl;
    auto s_seihr = Simulator(seihr_system, 100);
    s_seihr.simulate(ips_seihr_species_monitor);

    auto plot = plot_t("Trajectory of SEIHR (N=10'000)", "Time, days", "Count", 1920, 1080);
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
    auto s_simple = Simulator(simple_system, 100'000);
    s_simple.simulate(ips_species_monitor);

    auto plot_simple = plot_t("Trajectory of Simple (A=100, B=0, C=2)", "Time", "Count", 1920, 1080);
    for (const auto& [species, quantities] : *trajectoryMonitor.speciesQuantities) {
        std::string speciesName = species.getName();

        plot_simple.lines(speciesName, *trajectoryMonitor.timePoints, quantities);
    }

    plot_simple.process();
    plot_simple.save_to_png("simple.png");
}

double run_seihr_simulation(size_t N) {
    auto seihr_system = seihr(N);

    auto s_seihr = Simulator(seihr_system, 100);
    auto h_mon = SpeciesPeakMonitor("H");

    s_seihr.simulate(h_mon);

    return *h_mon.speciesPeak;
}

template <typename Func>
void perform_simulation(const size_t N, const size_t num_simulations, ThreadPool& thread_pool, Func operate_on_results) {
    std::vector<std::future<double>> futures;
    std::vector<double> results;

    for(int i = 0; i < num_simulations; ++i) {
        futures.push_back(thread_pool.enqueue(run_seihr_simulation, N));
    }

    for(auto &f : futures) {
        results.push_back(f.get());
    }

    operate_on_results(results);
}
double calculate_mean(const std::vector<double>& xs) {
    return std::accumulate(xs.begin(), xs.end(), 0.0) / xs.size();
}

double calculate_peak(const std::vector<double>& xs) {
    return *std::max_element(xs.begin(), xs.end());
}

void peak_avg_seihr(size_t num_simulations, size_t concurrency_level) {
    auto begin = std::chrono::steady_clock::now();
    const size_t N_NJ = 589'755;
    const size_t N_DK = 5'882'763;

    std::cout << "Simulating SEIHR..." << std::endl;

    ThreadPool thread_pool(concurrency_level);

    perform_simulation(N_NJ, num_simulations, thread_pool, [&](std::vector<double> const& results) {
        double avg_peak = calculate_mean(results);
        std::cout << "Average peak of Hospitalized in NJ over " << num_simulations << " simulations: " << avg_peak << std::endl;
    });

    perform_simulation(N_DK, num_simulations, thread_pool, [&](std::vector<double> const& results) {
        double max_peak = calculate_peak(results);
        std::cout << "Maximum peak of Hospitalized in DK over " << num_simulations << " simulations: " << max_peak << std::endl;
    });

    auto end = std::chrono::steady_clock::now();
    std::cout << "Time elapsed for " << num_simulations << " w. CL " << concurrency_level << " = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}


int main(int argc, char const *argv[])
{
    //make_graphs();
    do_benchmarks();
    //plot_simple();
    //plot_circadian();
    //plot_seihr();
    //peak_avg_seihr(10, 12);
    //benchmark_plot();

    return 0;
}
