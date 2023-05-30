#ifndef STOCHASTIC_SIMULATOR
#define STOCHASTIC_SIMULATOR
#include <iostream>
#include <random>
#include <chrono>
#include <limits>
#include <memory>

#include "types.cpp"

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
        // start tracking time
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        while (t <= end_time_) {
            for (auto &r : reactions) {
                const auto delay = compute_delay(r);
                r.setDelay(delay);
            }

            auto next_reaction = find_min_delay_reaction(reactions);
            if (!next_reaction) {
                break; // No more reactions can proceed
            }

            t += next_reaction->delay();;

            if (!can_react(*next_reaction)) {
                continue;
            }

            react(*next_reaction);
            monitor(system_, t);
        }

        // end tracking time
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    }

private:
    System system_;
    double end_time_;
    std::default_random_engine generator_;

    double compute_delay(const Reaction &r) {
        double lambda_k = r.rate();

        // λk = λ * ∏i Ri,k
        for (const auto &reactant : r.reactants) {
            lambda_k *= system_.amount(reactant);
        }

        std::exponential_distribution<double> distribution(lambda_k);
        return distribution(generator_);
    }

    std::shared_ptr<Reaction> find_min_delay_reaction(const std::vector<Reaction> &reactions) {
        double min_delay = std::numeric_limits<double>::max();
        std::shared_ptr<Reaction> min_reaction = nullptr;

        for (auto const &reaction : reactions) {
            if (reaction.delay() < min_delay) {
                min_reaction = std::make_shared<Reaction>(reaction);
                min_delay = reaction.delay();
            }
        }

        return min_reaction;
    }

    bool can_react(const Reaction &r) {
        for (size_t i = 0; i < r.reactants.size(); ++i) {
            auto species_amount = system_.amount(r.reactants.at(i));

            // Currently assuming only 1 reactant is needed
            if (species_amount < 1) {
                return false;
            }
        }
        return true;
    }

    void react(Reaction &r) {
        for (size_t i = 0; i < r.reactants.size(); ++i) {
            // Currently assuming only 1 reactant is consumed
            const double amount = system_.amount(r.reactants[i]) - 1;
            system_.setAmount(r.reactants[i], amount);
        }

        for (size_t i = 0; i < r.products.size(); ++i) {
            // Currently assuming only 1 product is produced
            const double amount = system_.amount(r.products[i]) + 1;
            system_.setAmount(r.products[i], amount);
        }
    }

    void print_state() {

        for (auto species = system_.getSpecies(); const auto &[s, amount] : species) {
            std::cout << s.getName() << ": " << amount << " " << std::endl;
        }

        std::cout << '\n';
    }
};
#endif