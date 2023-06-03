#include "peak_avg_seihr.h"
#include "../parallel_simulator.h"

double run_seihr_simulation(size_t N) {
    auto seihr_system = seihr(N);

    auto s_seihr = Simulator(seihr_system, 100);
    auto h_mon = SpeciesPeakMonitor("H");

    s_seihr.simulate(h_mon);

    return *h_mon.speciesPeak;
}

template <typename Func>
void perform_parallel_simulations(const size_t N, const size_t num_simulations, size_t concurrency_level, Func operate_on_results) {
    auto system_factory = [&N]() { return seihr(N); };
    auto monitor_factory = []() { return std::make_unique<SpeciesPeakMonitor>("H"); };

    ParallelSimulator<SpeciesPeakMonitor> parallel_simulator(system_factory, monitor_factory, 100, num_simulations, concurrency_level);

    parallel_simulator.simulate();

    std::vector<double> results;
    for (auto& monitor : parallel_simulator.getMonitors()) {
        results.push_back(*monitor->speciesPeak);
    }

    operate_on_results(results);
}

double calculate_mean(const std::vector<double>& xs) {
    return std::accumulate(xs.begin(), xs.end(), 0.0) / xs.size();
}

double calculate_peak(const std::vector<double>& xs) {
    return *std::max_element(xs.begin(), xs.end());
}

// Solution to second part of requirement 7: Use it to estimate
// the peak of hospitalized agents in Covid-19 example without storing trajectory data for NNJ and NDK.
void calculate_peak_and_avg_seihr(size_t num_simulations, size_t concurrency_level) {
    auto begin = std::chrono::steady_clock::now();
    const size_t N_NJ = 589'755;
    const size_t N_DK = 5'882'763;

    std::cout << "Simulating SEIHR..." << std::endl;

    perform_parallel_simulations(N_NJ, num_simulations, concurrency_level, [&](std::vector<double> const &results) {
        double avg_peak = calculate_mean(results);
        std::cout << "Average peak of Hospitalized in NJ over " << num_simulations << " simulations: " << avg_peak
                  << std::endl;
    });

    perform_parallel_simulations(N_DK, num_simulations, concurrency_level, [&](std::vector<double> const &results) {
        double max_peak = calculate_peak(results);
        std::cout << "Maximum peak of Hospitalized in DK over " << num_simulations << " simulations: " << max_peak
                  << std::endl;
    });

    auto end = std::chrono::steady_clock::now();
    std::cout << "Time elapsed for " << num_simulations << " w. CL " << concurrency_level << " = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}
