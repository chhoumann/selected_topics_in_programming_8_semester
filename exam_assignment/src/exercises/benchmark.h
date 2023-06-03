#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "../examples/examples.h"
#include "../plot/plot.hpp"
#include "../stochastic_simulator.h"
#include "../thread_pool.h"
#include <chrono>
#include <vector>
#include <future>
#include <map>
#include <functional>
#include <string>

using LevelSimulationsMap = std::map<size_t, std::map<size_t, double>>;
using Results = std::vector<double>;

class BenchmarkPlotter {
public:
    BenchmarkPlotter(const std::string& title, const std::string& xlabel, const std::string& ylabel, int width, int height);
    void addLine(const LevelSimulationsMap& data);
    void save(const std::string& filename);

private:
    plot_t plot_;
};

class Benchmark {
public:
    using Task = std::function<void()>;

    Benchmark(
            const std::vector<size_t>& numSimulations,
            const std::vector<size_t>& concurrencyLevels,
            Task task,
            int numRepeats = 5);

    void Run();

    const LevelSimulationsMap& GetAverageRuntimes() const;
    const LevelSimulationsMap& GetTotalRuntimes() const;

private:
    std::vector<size_t> numSimulations_;
    std::vector<size_t> concurrencyLevels_;
    Task task_;
    int numRepeats_;
    LevelSimulationsMap average_runtimes, average_total_runtimes;

    double calculateAverage(const Results& results);
    double runAndTimeTask();
    Results performSimulations(size_t concurrency_level, size_t num_simulation);
    void performSimulationsAndStoreResults(size_t concurrency_level, size_t num_simulation);
    void addResultToMap(size_t concurrency_level, size_t num_simulations, double result, LevelSimulationsMap& map);
};

void do_benchmarks();

#endif //BENCHMARK_H
