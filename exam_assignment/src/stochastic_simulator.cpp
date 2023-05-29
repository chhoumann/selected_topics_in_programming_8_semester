#ifndef STOCHASTIC_SIMULATOR
#define STOCHASTIC_SIMULATOR
#include <iostream>
#include <random>
#include <chrono>
#include <limits>
#include <memory>
#include <unordered_map>

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
        std::cout << "Simulating..." << std::endl;
        double t = 0;

        auto& reactions = system_.getReactions();
        std::unordered_map<Reaction, double, ReactionHash> reaction_delays;

        while (t <= end_time_) {
            std::cout << "t = " << t << std::endl;
            for (auto &r : reactions) {
                const auto delay = compute_delay(r);
                reaction_delays[r] = delay;
            }

            auto next_reaction = find_min_delay_reaction(reaction_delays);
            if (!next_reaction) {
                std::cout << "No more reactions can proceed" << std::endl;
                break; // No more reactions can proceed
            }
            std::cout << "Next reaction: " << *next_reaction << std::endl;
            t += next_reaction->rate;
            std::cout << "t = " << t << std::endl;
            if (can_react(*next_reaction)) {
                std::cout << "Reacting..." << std::endl;
                react(*next_reaction);
            }

            std::cout << "State: " << std::endl;
            print_state();
        }
    }

private:
    System system_;
    double end_time_;
    std::default_random_engine generator_;

    double compute_delay(const Reaction &r) {
        double lambda_k = r.rate;

        // λk = λ * ∏i Ri,k
        for (const auto &reactant : r.reactants) {
            lambda_k *= system_.getAmount(reactant);
        }

        std::exponential_distribution<double> distribution(lambda_k);
        return distribution(generator_);
    }

    std::shared_ptr<Reaction> find_min_delay_reaction(std::unordered_map<Reaction, double, ReactionHash> &reaction_delays) {
        double min_delay = std::numeric_limits<double>::max();
        std::shared_ptr<Reaction> min_reaction = nullptr;

        for (auto &[reaction, delay] : reaction_delays) {
            if (delay < min_delay) {
                min_reaction = std::make_shared<Reaction>(reaction);
                min_delay = delay;
            }
        }

        return min_reaction;
    }

    bool can_react(const Reaction &r) {
        std::cout << "Checking if reaction can proceed..." << std::endl;
        std::cout << "Reaction: " << r << std::endl;
        std::cout << "Reactants Size: " << r.reactants.size() << std::endl;

        for (size_t i = 0; i < r.reactants.size(); ++i) {
            std::cout << "What the fuck just do it" << std::endl; //<< r.reactants[i].getName() << " >= " << r.delta_R[i] << std::endl;
            auto species_amount = system_.getAmount(r.reactants.at(i));
            std::cout << "amount: " << species_amount << std::endl;

            if (species_amount < r.delta_R[i]) {
                std::cout << "Reaction cannot proceed" << std::endl;
                return false;
            }
        }
        std::cout << "Reaction can proceed" << std::endl;
        return true;
    }

    void react(Reaction &r) {
        for (size_t i = 0; i < r.reactants.size(); ++i) {
            const double amount = system_.getAmount(r.reactants[i]) - r.delta_R[i];
            system_.setAmount(r.reactants[i], amount);
        }

        for (size_t i = 0; i < r.products.size(); ++i) {
            const double amount = system_.getAmount(r.products[i]) + r.delta_P[i];
            system_.setAmount(r.products[i], amount);
        }
    }

    void print_state() {
        auto species = system_.getSpecies();

        for (const auto &[s, amount] : species) {
            std::cout << s.getName() << ": " << amount << " ";
        }

        std::cout << '\n';
    }
};
#endif