#include "stochastic_simulator.h"

// Solves requirement 4: Implement the stochastic simulation (Alg. 1) of the system using the reaction rules.

double Simulator::compute_delay(const Reaction &r) {
    double lambda_k = r.rate();

    // λk = λ * ∏i Ri,k
    for (const auto &reactant : r.reactants) {
        lambda_k *= system_.amount(reactant);
    }

    // These two lines generate a random number from an exponential distribution
    std::exponential_distribution<double> distribution(lambda_k); // Models the exponential distribution
    return distribution(generator_); // Returns a random number from the distribution, using the generator - which generates uniformly distributed, pseudo-random numbers
}

std::shared_ptr<Reaction> Simulator::find_min_delay_reaction(const std::vector<Reaction> &reactions) {
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

bool Simulator::can_react(const Reaction &r) {
    for (size_t i = 0; i < r.reactants.size(); ++i) {
        auto species_amount = system_.amount(r.reactants.at(i));

        // Currently assuming only 1 reactant is needed
        if (species_amount < 1) {
            return false;
        }
    }
    return true;
}

void Simulator::react(Reaction &r) {
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
