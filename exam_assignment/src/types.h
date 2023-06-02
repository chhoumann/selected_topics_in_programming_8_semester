#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <utility>
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>

class Species {
public:
    explicit Species(std::string name);

    [[nodiscard]] const std::string &getName() const;

    bool operator<(const Species& other) const;

private:
    std::string _name;
};

std::vector<Species> operator+(const Species &lhs, const Species &rhs);

class Reaction {
public:
    std::vector<Species> reactants;
    std::vector<Species> products;

    double rate() const;
    double delay() const;

    void setDelay(double delay);

    std::vector<double> delta_R;
    std::vector<double> delta_P;

    Reaction(Reaction &&other, double rate);
    Reaction(std::vector<Species> &&reactants, std::vector<Species> &&products);

private:
    double rate_;
    double delay_;
};

Reaction operator>>=(const Species &reactant, const Species &product);
Reaction operator>>=(std::vector<Species> &&reactants, const Species &product);
Reaction operator>>=(const Species &reactant, std::vector<Species> &&products);
Reaction operator>>=(std::vector<Species> &&reactants, std::vector<Species> &&products);

std::ostream& operator<<(std::ostream& os, const Reaction& reaction);

struct ReactionHash {
    std::size_t operator()(const Reaction& r) const;
};

bool operator==(const Reaction& lhs, const Reaction& rhs);

class System {
public:
    [[nodiscard]] const std::map<Species, int> &getSpecies() const;
    [[nodiscard]] std::vector<Reaction> &getReactions();

    Species operator()(const std::string &name, double amount);
    Reaction operator()(Reaction &&r, double rate);
    Species environment();

    int amount(const Species &s) const;
    void setAmount(const Species &s, int amount);

private:
    std::map<Species, int> species;
    std::vector<Reaction> reactions;
};

#endif // TYPES_H
