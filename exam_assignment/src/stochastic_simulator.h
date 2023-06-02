#ifndef STOCHASTIC_SIMULATOR_H
#define STOCHASTIC_SIMULATOR_H

#include <iostream>
#include <random>
#include <chrono>
#include <limits>
#include <memory>

#include "types.h"

class Simulator {
public:
    Simulator(const System& system, double end_time)
            : system_(system)
            , end_time_(end_time)
            , generator_(std::chrono::system_clock::now().time_since_epoch().count())
    {
    }

    template<typename Monitor>
    void simulate(Monitor monitor) {
        double t = 0;

        auto& reactions = system_.getReactions();

        while (t <= end_time_) {
            for (auto &r : reactions) {
                const auto delay = compute_delay(r);
                r.setDelay(delay);
            }

            auto next_reaction = find_min_delay_reaction(reactions);
            if (!next_reaction) {
                break; // No more reactions can proceed
            }

            t += next_reaction->delay();

            if (!can_react(*next_reaction)) {
                continue;
            }

            react(*next_reaction);
            monitor(system_, t);
        }
    }

    double compute_delay(const Reaction &r);
    std::shared_ptr<Reaction> find_min_delay_reaction(const std::vector<Reaction> &reactions);
    bool can_react(const Reaction &r);
    void react(Reaction &r);

private:
    System system_;
    double end_time_;
    std::default_random_engine generator_;
};
#endif
