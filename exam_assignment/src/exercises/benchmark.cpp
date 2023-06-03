#include "benchmark.h"

BenchmarkPlotter::BenchmarkPlotter(const std::string& title, const std::string& xlabel, const std::string& ylabel, int width, int height)
        : plot_(title, xlabel, ylabel, width, height) {}

void BenchmarkPlotter::addLine(const LevelSimulationsMap& data) {
    for (const auto& [concurrency_level, results] : data) {
        std::vector<double> x(results.size());
        std::vector<double> y(results.size());
        size_t i = 0;
        for (const auto& [num_simulation, avg_time] : results) {
            x[i] = static_cast<double>(num_simulation);
            y[i] = avg_time;
            ++i;
        }

        plot_.lines("Threads: " + std::to_string(concurrency_level), x, y);
    }
}

void BenchmarkPlotter::save(const std::string& filename) {
    plot_.process();
    plot_.save_to_png(filename);
}

Benchmark::Benchmark(
        const std::vector<size_t>& numSimulations,
        const std::vector<size_t>& concurrencyLevels,
        Task task,
        int numRepeats)
        : numSimulations_(numSimulations),
          concurrencyLevels_(concurrencyLevels),
          task_(std::move(task)),
          numRepeats_(numRepeats) {}

void Benchmark::Run() {
    for (auto concurrency_level : concurrencyLevels_) {
        for (auto num_simulation : numSimulations_) {
            performSimulationsAndStoreResults(concurrency_level, num_simulation);
        }

        std::cout << "Done with concurrency level " << concurrency_level << "." << std::endl;
    }
}

const LevelSimulationsMap& Benchmark::GetAverageRuntimes() const {
    return average_runtimes;
}

const LevelSimulationsMap& Benchmark::GetTotalRuntimes() const {
    return average_total_runtimes;
}

double Benchmark::runAndTimeTask() {
    auto start = std::chrono::high_resolution_clock::now();
    task_();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

Results Benchmark::performSimulations(size_t concurrency_level, size_t num_simulation) {
    Results results;
    for (int repeat = 0; repeat < numRepeats_; ++repeat) {
        ThreadPool tp(concurrency_level);
        std::vector<std::future<double>> futures;
        for (int i = 0; i < num_simulation; ++i) {
            futures.push_back(tp.enqueue([this]{ return runAndTimeTask(); }));
        }

        for (auto &f : futures) {
            results.push_back(f.get());
        }
    }
    return results;
}

void Benchmark::performSimulationsAndStoreResults(size_t concurrency_level, size_t num_simulation) {
    auto begin_total = std::chrono::steady_clock::now();

    Results results = performSimulations(concurrency_level, num_simulation);
    double average = calculateAverage(results);
    addToMapAndLog(concurrency_level, num_simulation, average, average_runtimes, "Average time");

    auto total = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin_total).count() / 1000;
    addToMapAndLog(concurrency_level, num_simulation, total, average_total_runtimes, "Total time");
}

double Benchmark::calculateAverage(const Results& results) {
    return std::accumulate(results.begin(), results.end(), 0.0) / results.size();
}

void Benchmark::addToMapAndLog(size_t concurrency_level, size_t num_simulation, double average, LevelSimulationsMap& map, const std::string& description) {
    map[concurrency_level][num_simulation] = average;
    std::cout << description << " for " << num_simulation << " w. CL " << concurrency_level << " = " << average << std::endl;
}

void do_benchmarks() {
    auto task = []() {
        auto seihr_system = seihr(10'000);

        auto s_seihr = Simulator(seihr_system, 100);
        auto emptyLambda = [](const auto&, const auto&){};

        auto begin = std::chrono::steady_clock::now();
        s_seihr.simulate(emptyLambda);
    };

    Benchmark benchmark({10, 20, 30, 40, 50}, {1, 2, 3, 4, 5}, task);
    benchmark.Run();

    {
        BenchmarkPlotter plotAvg("Benchmark Results - Averages", "Simulations", "Average Sim Time (ms) - avg. of n * 5", 1920, 1080);
        plotAvg.addLine(benchmark.GetAverageRuntimes());
        plotAvg.save("avg_sim_benchmark_results.png");
    }

    {
        BenchmarkPlotter plotTotal("Benchmark Results - Totals", "Simulations", "Total Sim Time (s) - avg. of n * 5", 1920, 1080);
        plotTotal.addLine(benchmark.GetTotalRuntimes());
        plotTotal.save("total_sim_benchmark_results.png");
    }
}