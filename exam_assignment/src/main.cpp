#include <iostream>
#include <tuple>

#include "types.cpp"
#include "graph_generator.cpp"
#include "stochastic_simulator.cpp"
#include "plot/plot.cpp"
#include "monitor/monitor.cpp"
#include "thread_pool.cpp"

#include "examples/seihr.cpp"
#include "examples/circadian_oscillator.cpp"
#include "examples/simple.cpp"

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
    auto seihr_system = seihr(10'000); // 589'755 for DK_NJ
    auto trajectoryMonitor = SpeciesTrajectoryMonitor();
    auto ips_seihr_species_monitor = make_ips_counter(trajectoryMonitor);

    std::cout << "Simulating SEIHR..." << std::endl;
    auto s_seihr = Simulator(seihr_system, 100);
    s_seihr.simulate(ips_seihr_species_monitor);

    auto plot = plot_t("Trajectory of SEIHR (N=10'000)", "Time, days", "Count", 1920, 1080);
    for (const auto& [species, quantities] : *trajectoryMonitor.speciesQuantities) {
        std::string speciesName = species.getName();

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

double run_seihr_simulation(int N) {
    auto seihr_system = seihr(N);

    auto s_seihr = Simulator(seihr_system, 100);
    auto h_mon = SpeciesPeakMonitor("H");

    s_seihr.simulate(h_mon);

    return *h_mon.speciesPeak;
}

void peak_seihr(int num_simulations, size_t concurrency_level) {
    auto begin = std::chrono::steady_clock::now();
    const int N_NJ = 589'755;
    const int N_DK = 5'882'763;

    std::cout << "Simulating SEIHR..." << std::endl;

    ThreadPool thread_pool(concurrency_level);

    std::vector<std::future<double>> futures_nj;
    std::vector<std::future<double>> futures_dk;

    for(int i = 0; i < num_simulations; ++i) {
        futures_nj.push_back(thread_pool.enqueue(run_seihr_simulation, N_NJ));
        futures_dk.push_back(thread_pool.enqueue(run_seihr_simulation, N_DK));
    }

    std::vector<double> results_nj;
    std::vector<double> results_dk;

    for(auto &f : futures_nj) {
        results_nj.push_back(f.get());
    }
    for(auto &f : futures_dk) {
        results_dk.push_back(f.get());
    }

    auto end = std::chrono::steady_clock::now();
    std::cout << "Time elapsed for " << num_simulations << " w. CL " << concurrency_level << " = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

    double avg_peak_nj = std::accumulate(results_nj.begin(), results_nj.end(), 0.0) / results_nj.size();
    double avg_peak_dk = std::accumulate(results_dk.begin(), results_dk.end(), 0.0) / results_dk.size();
    std::cout << "Average peak of Hospitalized in NJ over " << num_simulations << " simulations: " << avg_peak_nj << std::endl;
    std::cout << "Average peak of Hospitalized in DK over " << num_simulations << " simulations: " << avg_peak_dk << std::endl;

    double max_peak_nj = *std::max_element(results_nj.begin(), results_nj.end());
    double max_peak_dk = *std::max_element(results_dk.begin(), results_dk.end());
    std::cout << "Maximum peak of Hospitalized in NJ over " << num_simulations << " simulations: " << max_peak_nj << std::endl;
    std::cout << "Maximum peak of Hospitalized in DK over " << num_simulations << " simulations: " << max_peak_dk << std::endl;
}

// returns time in milliseconds
double time_simulation() {
    auto seihr_system = seihr(10'000);

    auto s_seihr = Simulator(seihr_system, 100);
    auto emptyLambda = [](const auto&, const auto&){};

    auto begin = std::chrono::steady_clock::now();
    s_seihr.simulate(emptyLambda);
    auto end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}

void benchmark_plot() {
    std::vector<size_t> num_simulations = { 10, 100, 1000, 10000 };
    std::vector<size_t> concurrency_levels = { 1, 3, 5, 10, 12, 15, 20 };

    // (Concurrency level -> (Num simulations -> average time))
    std::map<size_t, std::map<size_t, double>> averages;

    // (Concurrency level -> (Num simulations -> total time))
    std::map<size_t, std::map<size_t, double>> avg_totals;

    for (auto concurrency_level : concurrency_levels) {
        for (auto num_simulation : num_simulations) {
            std::vector<double> avg_results;
            std::vector<double> totals;

            for (int repeat = 0; repeat < 5; ++repeat) {
                ThreadPool tp(concurrency_level);
                std::vector<std::future<double>> futures;

                for (int i = 0; i < num_simulation; ++i) {
                    futures.push_back(tp.enqueue(time_simulation));
                }

                std::vector<double> results;

                for (auto &f : futures) {
                    results.push_back(f.get());
                }

                // Calculate average for each sim run
                double average = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
                avg_results.push_back(average);

                // Calculate total for sim run & convert to seconds
                double total = std::accumulate(results.begin(), results.end(), 0.0) / 1000;
                totals.push_back(total);
            }

            // Add average of averages to the averages map
            averages[concurrency_level][num_simulation] = std::accumulate(avg_results.begin(), avg_results.end(), 0.0) / avg_results.size();
            std::cout << "Average time for " << num_simulation << " w. CL " << concurrency_level << " = " << averages[concurrency_level][num_simulation] << "ms" << std::endl;

            // Add average of totals to the totals map
            avg_totals[concurrency_level][num_simulation] = std::accumulate(totals.begin(), totals.end(), 0.0) / totals.size();
            std::cout << "Total time for " << num_simulation << " w. CL " << concurrency_level << " = " << avg_totals[concurrency_level][num_simulation] << "s" << std::endl;
        }

        std::cout << "Done with concurrency level " << concurrency_level << "." << std::endl;
    }

    plot_t plot_avg("Benchmark Results - Averages", "Simulations", "Average Sim Time (ms) - avg. of n * 5", 1920, 1080);

    for (const auto& [concurrency_level, results] : averages) {
        // Convert map keys (num_simulations) and values (average times) to vectors
        std::vector<double> x(results.size()), y(results.size());
        size_t i = 0;
        for (const auto& [num_simulation, avg_time] : results) {
            x[i] = static_cast<double>(num_simulation);
            y[i] = avg_time;
            ++i;
        }

        plot_avg.lines("Threads: " + std::to_string(concurrency_level), x, y);
    }

    plot_avg.process();
    plot_avg.save_to_png("avg_sim_benchmark_results.png");

    plot_t plot_total("Benchmark Results - Totals", "Simulations", "Total Sim Time (s) - avg. of n * 5", 1920, 1080);

    for (const auto& [concurrency_level, results] : avg_totals) {
        // Convert map keys (num_simulations) and values (total times) to vectors
        std::vector<double> x(results.size()), y(results.size());
        size_t i = 0;
        for (const auto& [num_simulation, t_time] : results) {
            x[i] = static_cast<double>(num_simulation);
            y[i] = t_time;
            ++i;
        }

        plot_total.lines("Threads: " + std::to_string(concurrency_level), x, y);
    }

    plot_total.process();
    plot_total.save_to_png("total_sim_benchmark_results.png");
}

int main(int argc, char const *argv[])
{
    //make_graphs();
    //plot_simple();
    //plot_circadian();
    //plot_seihr();
    //peak_seihr(100, 20);
    benchmark_plot();
}
