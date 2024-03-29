#ifndef PEAK_AVG_SEIHR_H
#define PEAK_AVG_SEIHR_H

#include <vector>
#include <future>
#include <chrono>
#include <numeric>
#include <algorithm>
#include "../thread_pool.h"
#include "../examples/examples.h"
#include "../stochastic_simulator.h"
#include "../monitor/species_peak_monitor.h"
#include "../monitor/species_trajectory_monitor.h"

double run_seihr_simulation(size_t N);

template <typename Func>
void perform_parallel_simulations(const size_t N, const size_t num_simulations, size_t concurrency_level, Func operate_on_results);

double calculate_mean(const std::vector<double>& xs);

double calculate_peak(const std::vector<double>& xs);

void calculate_peak_and_avg_seihr(size_t num_simulations, size_t concurrency_level, size_t N);

#endif //PEAK_AVG_SEIHR_H
