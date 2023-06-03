#include "peak_avg_seihr.h"

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

// Solution to second part of requirement 7: Use it to estimate
// the peak of hospitalized agents in Covid-19 example without storing trajectory data for NNJ and NDK.
void calculate_peak_and_avg_seihr(size_t num_simulations, size_t concurrency_level) {
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
