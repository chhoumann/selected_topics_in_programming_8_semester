#ifndef SPECIES_PEAK_MONITOR_H
#define SPECIES_PEAK_MONITOR_H

#include <string>
#include <memory>
#include "monitor.h"
#include "../types.h"

class SpeciesPeakMonitor : public Monitor {
public:
    std::shared_ptr<double> speciesPeak;

    explicit SpeciesPeakMonitor(std::string targetSpeciesName);

    void operator()(const System& system, double t) override;

private:
    std::string targetSpeciesName;
};

#endif  // SPECIES_PEAK_MONITOR_H
