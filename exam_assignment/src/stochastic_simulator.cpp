#include <iostream>
#include <random>
#include <string>

#include "types.cpp"

// Can also seed
// std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
std::default_random_engine generator;

void compute_delay(Reaction &r, double lambda)
{
    double lambda_k = lambda;

    // λk = λ * ∏i Ri,k
    for (const auto &reactant : r.reactants)
    {
        lambda_k *= reactant.amount;
    }

    std::exponential_distribution<double> distribution(lambda_k);

    r.delay = distribution(generator);
}

Reaction *find_min_delay_reaction(std::vector<Reaction> &reactions)
{
    // Find the reaction with minimum delay
    double min_delay = std::numeric_limits<double>::max();
    Reaction *min_reaction = nullptr;
    for (auto &r : reactions)
    {
        if (r.delay < min_delay)
        {
            min_delay = r.delay;
            min_reaction = &r;
        }
    }
    return min_reaction;
}

bool can_react(const Reaction &r)
{
    // Check if the reaction can proceed
    for (size_t i = 0; i < r.reactants.size(); ++i)
    {
        if (r.reactants[i].amount < r.delta_R[i])
        {
            return false;
        }
    }
    return true;
}

void react(Reaction &r)
{
    // Execute the reaction
    for (size_t i = 0; i < r.reactants.size(); ++i)
    {
        r.reactants[i].amount -= r.delta_R[i];
    }
    for (size_t i = 0; i < r.products.size(); ++i)
    {
        r.products[i].amount += r.delta_P[i];
    }
}

void print_state(const std::vector<Species> &species)
{
    // Print the state of the system
    for (const auto &s : species)
    {
        std::cout << s.amount << ' ';
    }
    std::cout << '\n';
}

void simulate(std::vector<Reaction> &reactions, std::vector<Species> &species, double end_time, double lambda)
{
    double t = 0;

    while (t <= end_time)
    {
        for (auto &r : reactions)
        {
            compute_delay(r, lambda);
        }

        Reaction *next_reaction = find_min_delay_reaction(reactions);
        if (next_reaction == nullptr)
        {
            break; // No more reactions can proceed
        }
        t += next_reaction->delay;
        if (can_react(*next_reaction))
        {
            react(*next_reaction);
        }
        print_state(species);
    }
}
