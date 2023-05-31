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

class IPSCounterMonitor : public Monitor {
public:
    explicit IPSCounterMonitor(std::function<void(const System&, double)> func) : func(std::move(func)) {}

    void operator()(const System& system, double t) override {
        static auto lastSecond = std::chrono::steady_clock::now();
        static int iterations = 0;

        func(system, t);
        iterations++;

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastSecond).count() >= 1) {
            std::cout << "Iterations per second: " << iterations / 1000 << "k" << '\n';
            iterations = 0;
            lastSecond = now;
        }
    }

private:
    std::function<void(const System&, double)> func;
};
