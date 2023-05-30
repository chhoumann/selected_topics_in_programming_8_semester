#ifndef STOCHASTIC_SIMULATOR
#define STOCHASTIC_SIMULATOR
#include <iostream>
#include <random>
#include <chrono>
#include <limits>
#include <memory>
#include <unordered_map>
#include <queue>
#include <ranges>
#include <algorithm>

#include "types.cpp"

class Simulator {
public:
    Simulator(const System& system, double end_time)
            : system_(system)
            , end_time_(end_time)
            , generator_(std::chrono::system_clock::now().time_since_epoch().count())
    {
    }

    void simulate() {
        double t = 0;
        auto reactions = system_.getReactions();

        // Start tracking time
        auto begin = std::chrono::steady_clock::now();

        while (t <= end_time_) {
            // Recompute delays and sort the reactions by delay
            for (auto &r : reactions) {
                r.setDelay(compute_delay(r));
            }
            std::ranges::sort(reactions, [](const Reaction& a, const Reaction& b) {
                return a.delay() < b.delay();
            });

            // Find the next reaction that can actually react
            auto next_reaction_it = std::ranges::find_if(reactions, [this](const Reaction& r) {
                return can_react(r);
            });

            // Break if no reaction can proceed
            if (next_reaction_it == reactions.end()) {
                break;
            }

            // React and advance time
            react(*next_reaction_it);
            t += next_reaction_it->delay();
        }

        // End tracking time
        auto end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
    }

private:
    System system_;
    double end_time_;
    std::default_random_engine generator_;

    double compute_delay(Reaction &r) {
        double lambda_k = r.rate();

        if (can_react(r)) { // compute delay only for reactions that can actually react
            for (const auto &reactant : r.reactants) {
                lambda_k *= system_.amount(reactant);
            }
            std::exponential_distribution<double> distribution(lambda_k);
            return distribution(generator_);
        } else {
            return std::numeric_limits<double>::max();  // return maximum possible delay for reactions that cannot react
        }
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