#include <map>
#include <vector>

class Monitor {
public:
    virtual ~Monitor() = default;

    virtual void operator()(const System& system, double t) = 0;
};

class SpeciesTrajectoryMonitor : public Monitor {
public:
    SpeciesTrajectoryMonitor() = default;

    void operator()(const System& system, double t) override {
        const auto& allSpecies = system.getSpecies();

        timePoints.push_back(t);

        for (const auto& [species, quantity] : allSpecies) {
            speciesQuantities[species].push_back(quantity);
        }
    }

    [[nodiscard]] const std::vector<double>& getTimePoints() const {
        return timePoints;
    }

    [[nodiscard]] const std::map<Species, std::vector<double>>& getSpeciesQuantities() const {
        return speciesQuantities;
    }

private:
    std::vector<double> timePoints;
    std::map<Species, std::vector<double>> speciesQuantities;
};

class SpeciesPeakMonitor : public Monitor {
public:
    std::shared_ptr<double> speciesPeak = std::make_shared<double>(0.0);

    SpeciesPeakMonitor(const std::string& targetSpeciesName) : targetSpeciesName(targetSpeciesName) {}

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

