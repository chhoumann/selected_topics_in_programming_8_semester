#include "species_peak_monitor.h"

SpeciesPeakMonitor::SpeciesPeakMonitor(std::string targetSpeciesName)
        : targetSpeciesName(std::move(targetSpeciesName)), speciesPeak(std::make_shared<double>(0.0)) {}

void SpeciesPeakMonitor::operator()(const System& system, double t) {
    const auto& allSpecies = system.getSpecies();

    for (const auto& [species, quantity] : allSpecies) {
        if (species.getName() == targetSpeciesName && quantity > *speciesPeak) {
            *speciesPeak = quantity;
        }
    }
}
