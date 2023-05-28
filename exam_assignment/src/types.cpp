#ifndef TYPES
#define TYPES
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <stdexcept>

class Species
{
public:
    explicit Species(std::string name) : _name(std::move(name)) {}

    [[nodiscard]] const std::string &getName() const { return _name; }

    // For compatibility with std::map
    bool operator<(const Species& other) const {
        return _name < other._name;
    }

private:
    std::string _name;
};

std::vector<Species> operator+(const Species &lhs, const Species &rhs)
{
    return {lhs, rhs};
}

class Reaction
{
public:
    std::vector<Species> reactants;
    std::vector<Species> products;

    double delay;

    std::vector<double> delta_R;
    std::vector<double> delta_P;

    Reaction(Reaction &&other, double delay) : reactants(std::move(other.reactants)),
                                               products(std::move(other.products)), delay(delay), delta_R(std::move(other.delta_R)), delta_P(std::move(other.delta_P)) {}
    Reaction(std::vector<Species> &&reactants, std::vector<Species> &&products) : reactants(std::move(reactants)), products(std::move(products)) {}
};

Reaction operator>>=(const Species &reactant, const Species &product)
{
    return Reaction({reactant}, {product});
}

Reaction operator>>=(std::vector<Species> &&reactants, const Species &product)
{
    return Reaction(std::move(reactants), {product});
}

Reaction operator>>=(const Species &reactant, std::vector<Species> &&products)
{
    return Reaction({reactant}, std::move(products));
}

Reaction operator>>=(std::vector<Species> &&reactants, std::vector<Species> &&products)
{
    return Reaction(std::move(reactants), std::move(products));
}

class System
{
public:
    [[nodiscard]] const std::map<Species, int> &getSpecies() const { return species; }
    [[nodiscard]] const std::vector<Reaction> &getReactions() const { return reactions; }

    Species operator()(const std::string &name, double amount)
    {
        if (species.contains(Species(name)))
        {
            throw std::runtime_error("Species already exists");
        }

        auto s = Species(name);
        species.try_emplace(s, amount);

        return s;
    }

    void operator()(Reaction &&r, double rate)
    {
        auto re = Reaction(std::move(r), rate);

        reactions.push_back(re);
    }

    /// @brief Assuming the environment means we don't care to track the species
    /// @return Untracked species
    Species environment() const
    {
        return Species("environment");
    }
private:
    std::map<Species, int> species = {};
    std::vector<Reaction> reactions = {};
};
#endif