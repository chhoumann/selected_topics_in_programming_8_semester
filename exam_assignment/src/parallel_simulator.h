#ifndef PARALLEL_SIMULATOR_H
#define PARALLEL_SIMULATOR_H

#include <vector>
#include <functional>
#include <memory>
#include "stochastic_simulator.h"
#include "thread_pool.h"
#include "monitor/monitor.h"

template<typename MonitorType>
class ParallelSimulator {
public:
    using SystemFactory = std::function<System()>;
    using MonitorFactory = std::function<std::unique_ptr<MonitorType>()>;

    ParallelSimulator(SystemFactory system_factory, MonitorFactory monitor_factory, double end_time, size_t num_sims, size_t num_threads);

    void simulate();

    const std::vector<std::unique_ptr<MonitorType>>& getMonitors() const;

private:
    SystemFactory system_factory_;
    MonitorFactory monitor_factory_;
    double end_time_;
    size_t num_sims_;
    ThreadPool thread_pool_;
    std::vector<std::unique_ptr<MonitorType>> monitors_;
};

template<typename MonitorType>
ParallelSimulator<MonitorType>::ParallelSimulator(SystemFactory system_factory, MonitorFactory monitor_factory, double end_time, size_t num_sims, size_t num_threads)
        : system_factory_(std::move(system_factory)), monitor_factory_(std::move(monitor_factory)), end_time_(end_time), num_sims_(num_sims), thread_pool_(num_threads) {
            monitors_.reserve(num_sims);
        }

template<typename MonitorType>
void ParallelSimulator<MonitorType>::simulate() {
    std::vector<std::future<void>> futures;
    monitors_.clear();

    for (size_t i = 0; i < num_sims_; ++i) {
        monitors_.emplace_back(monitor_factory_());

        System system = system_factory_();

        auto monitor = monitors_.back().get();
        futures.emplace_back(thread_pool_.enqueue([this, system = std::move(system), monitor] {
            Simulator simulator(std::move(system), end_time_);
            simulator.simulate(*monitor);
        }));
    }

    for (auto& future : futures) {
        future.get();
    }
}

template<typename MonitorType>
const std::vector<std::unique_ptr<MonitorType>>& ParallelSimulator<MonitorType>::getMonitors() const {
    return monitors_;
}

#endif //PARALLEL_SIMULATOR_H
