#ifndef TYPES
#define TYPES
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>

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
    std::vector<Species> reactants = {};
    std::vector<Species> products = {};

    double rate() const { return rate_; }
    double delay() const { return delay_; }

    void setDelay(double delay) { delay_ = delay; }

    std::vector<double> delta_R;
    std::vector<double> delta_P;

    Reaction(Reaction &&other, double rate) : reactants(std::move(other.reactants)),
                                               products(std::move(other.products)), rate_(rate), delta_R(std::move(other.delta_R)), delta_P(std::move(other.delta_P)) {}
    Reaction(std::vector<Species> &&reactants, std::vector<Species> &&products) : reactants(std::move(reactants)), products(std::move(products)) {}

private:
    double rate_;
    double delay_;
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

std::ostream& operator<<(std::ostream& os, const Reaction& reaction) {
    os << reaction.reactants[0].getName();
    for (int i = 1; i < reaction.reactants.size(); i++) {
        os << " + " << reaction.reactants[i].getName();
    }

    os << " → " << reaction.products[0].getName();
    for (int i = 1; i < reaction.products.size(); i++) {
        os << " + " << reaction.products[i].getName();
    }

    os << " (rate: " << reaction.rate() << ")";

    return os;
}

struct ReactionHash {
    std::size_t operator()(const Reaction& r) const {
        std::size_t h = 0;

        for (auto& s : r.reactants) {
            h ^= std::hash<std::string>{}(s.getName());
        }
        for (auto& s : r.products) {
            h ^= std::hash<std::string>{}(s.getName());
        }

        h ^= std::hash<double>{}(r.rate());

        return h;
    }
};

bool operator==(const Reaction& lhs, const Reaction& rhs) {
    ReactionHash hash;

    return hash(lhs) == hash(rhs);
}

class System
{
public:
    [[nodiscard]] const std::map<Species, int> &getSpecies() const { return species; }
    [[nodiscard]] std::vector<Reaction> &getReactions() { return reactions; }

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

    int getAmount(const Species &s) const {
        if (!species.contains(s)) {
            throw std::runtime_error("Species does not exist");
        }

        return species.at(s);
    }

    void setAmount(const Species &s, int amount) {
        if (!species.contains(s)) {
            throw std::runtime_error("Species does not exist");
        }

        species[s] = amount;
    }
private:
    std::map<Species, int> species = {};
    std::vector<Reaction> reactions = {};
};
#endif