#ifndef SPECIES_TRAJECTORY_MONITOR_H
#define SPECIES_TRAJECTORY_MONITOR_H

#include <memory>
#include <vector>
#include <map>
#include "monitor.h"
#include "../types.h"

class SpeciesTrajectoryMonitor : public Monitor {
public:
    SpeciesTrajectoryMonitor();

    void operator()(const System& system, double t) override;

    std::shared_ptr<std::vector<double>> getTimePoints();
    std::shared_ptr<std::map<Species, std::vector<double>>> getSpeciesQuantities();

    std::shared_ptr<std::vector<double>> timePoints;
    std::shared_ptr<std::map<Species, std::vector<double>>> speciesQuantities;
};

#endif //SPECIES_TRAJECTORY_MONITOR_H
