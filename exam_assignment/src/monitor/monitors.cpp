#include "monitor.h"
#include "../types.h"

#include <map>
#include <utility>
#include <vector>
#include <memory>

class SpeciesTrajectoryMonitor : public Monitor {
public:
    SpeciesTrajectoryMonitor()
            : timePoints(std::make_shared<std::vector<double>>()),
              speciesQuantities(std::make_shared<std::map<Species, std::vector<double>>>()) {}

    void operator()(const System& system, double t) override {
        const auto& allSpecies = system.getSpecies();

        timePoints->push_back(t);

        for (const auto& [species, quantity] : allSpecies) {
            (*speciesQuantities)[species].push_back(quantity);
        }
    }

    std::shared_ptr<std::vector<double>> timePoints;
    std::shared_ptr<std::map<Species, std::vector<double>>> speciesQuantities;
};

class SpeciesPeakMonitor : public Monitor {
public:
    std::shared_ptr<double> speciesPeak = std::make_shared<double>(0.0);

    explicit SpeciesPeakMonitor(std::string targetSpeciesName) : targetSpeciesName(std::move(targetSpeciesName)) {}

    void operator()(const System& system, double t) override {
        const auto& allSpecies = system.getSpecies();

        for (const auto& [species, quantity] : allSpecies) {
            if (species.getName() == targetSpeciesName && quantity > *speciesPeak) {
                *speciesPeak = quantity;
            }
        }
    }

private:
    std::string targetSpeciesName;
};