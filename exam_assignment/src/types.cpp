#include "types.h"

Species::Species(std::string name) : _name(std::move(name)) {}

const std::string &Species::getName() const {
    return _name;
}

bool Species::operator<(const Species& other) const {
    return _name < other._name;
}

std::vector<Species> operator+(const Species &lhs, const Species &rhs) {
    return {lhs, rhs};
}

Reaction::Reaction(Reaction &&other, double rate) : reactants(std::move(other.reactants)),
                                                    products(std::move(other.products)), rate_(rate), delta_R(std::move(other.delta_R)), delta_P(std::move(other.delta_P)) {}

Reaction::Reaction(std::vector<Species> &&reactants, std::vector<Species> &&products) : reactants(std::move(reactants)), products(std::move(products)) {}

double Reaction::rate() const {
    return rate_;
}

double Reaction::delay() const {
    return delay_;
}

void Reaction::setDelay(double delay) {
    delay_ = delay;
}

Reaction operator>>=(const Species &reactant, const Species &product) {
    return Reaction({reactant}, {product});
}

Reaction operator>>=(std::vector<Species> &&reactants, const Species &product) {
    return Reaction(std::move(reactants), {product});
}

Reaction operator>>=(const Species &reactant, std::vector<Species> &&products) {
    return Reaction({reactant}, std::move(products));
}

Reaction operator>>=(std::vector<Species> &&reactants, std::vector<Species> &&products) {
    return Reaction(std::move(reactants), std::move(products));
}

// Solves requirement 1: Use operator overloading to support the reaction rule typesetting directly in C++ code.
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

std::size_t ReactionHash::operator()(const Reaction& r) const {
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

bool operator==(const Reaction& lhs, const Reaction& rhs) {
    ReactionHash hash;
    return hash(lhs) == hash(rhs);
}

const std::map<Species, int> &System::getSpecies() const {
    return species;
}

std::vector<Reaction> &System::getReactions() {
    return reactions;
}

Species System::operator()(const std::string &name, double amount) {
    if (species.contains(Species(name))) {
        throw std::runtime_error("Species already exists");
    }

    auto s = Species(name);
    species.try_emplace(s, amount);

    return s;
}

Reaction System::operator()(Reaction &&r, double rate) {
    auto re = Reaction(std::move(r), rate);
    reactions.push_back(re);
    return re;
}

Species System::environment() {
    const std::string environment = "environment";
    return (*this)(environment, 0.0);
}

int System::amount(const Species &s) const {
    if (!species.contains(s)) {
        auto name = s.getName();
        throw std::runtime_error("Species '" + name + "' does not exist");
    }
    return species.at(s);
}

void System::setAmount(const Species &s, int amount) {
    if (!species.contains(s)) {
        throw std::runtime_error("Species does not exist");
    }
    species[s] = amount;
}
