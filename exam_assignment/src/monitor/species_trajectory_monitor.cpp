#include "species_trajectory_monitor.h"

SpeciesTrajectoryMonitor::SpeciesTrajectoryMonitor()
        : timePoints(std::make_shared<std::vector<double>>()),
          speciesQuantities(std::make_shared<std::map<Species, std::vector<double>>>()) {}

void SpeciesTrajectoryMonitor::operator()(const System& system, double t) {
    const auto& allSpecies = system.getSpecies();

    timePoints->push_back(t);

    for (const auto& [species, quantity] : allSpecies) {
        (*speciesQuantities)[species].push_back(quantity);
    }
}

std::shared_ptr<std::vector<double>> SpeciesTrajectoryMonitor::getTimePoints() {
    return timePoints;
}

std::shared_ptr<std::map<Species, std::vector<double>>> SpeciesTrajectoryMonitor::getSpeciesQuantities() {
    return speciesQuantities;
}
