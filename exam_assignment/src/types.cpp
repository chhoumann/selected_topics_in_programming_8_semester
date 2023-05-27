#include <string>
#include <vector>

class Species
{
public:
    std::string name;
    double amount;

    Species(const std::string &name, double amount) : name(name), amount(amount) {}
};

Species operator+(const Species &lhs, const Species &rhs)
{
    return Species(lhs.name + " + " + rhs.name, lhs.amount + rhs.amount);
}

class Reaction
{
public:
    std::vector<Species> reactants;
    std::vector<Species> products;

    double delay;

    std::vector<double> delta_R;
    std::vector<double> delta_P;

    Reaction(Reaction &&other, double delay) : delay(delay), reactants(std::move(other.reactants)), products(std::move(other.products)), delta_R(std::move(other.delta_R)), delta_P(std::move(other.delta_P)) {}
    Reaction(std::vector<Species> &&reactants, std::vector<Species> &&products) : delay(delay), reactants(std::move(reactants)), products(std::move(products)) {}
};

Reaction operator>>=(const Species &lhs, const Species &rhs)
{
    return Reaction({lhs}, {rhs});
}

class vessel_t
{
public:
    std::vector<Species> species;
    std::vector<Reaction> reactions;

    Species operator()(const std::string &name, double amount)
    {
        // Or just throw if we're trying to add one that already exists?
        for (auto &s : species)
        {
            if (s.name == name)
            {
                s.amount += amount;
                return s;
            }
        }

        auto s = Species(name, amount);
        species.push_back(s);

        return s;
    }

    void operator()(Reaction &&r, double rate)
    {
        auto re = Reaction(std::move(r), rate);

        reactions.push_back(re);
    }

    Species environment()
    {
        return Species("environment", 1);
    }
};