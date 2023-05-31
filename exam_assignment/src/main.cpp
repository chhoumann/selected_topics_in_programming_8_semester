#include "types.cpp"
#include "examples/seihr.cpp"
#include "graph_generator.cpp"
#include "stochastic_simulator.cpp"
#include "examples/circadian_oscillator.cpp"
#include "examples/simple.cpp"
#include "plot/plot.cpp"
#include <iostream>
#include <functional>
#include <memory>
#include <tuple>
#include "monitor/monitor.cpp"

template <typename Func>
auto make_ips_counter(Func& func) {
    return [&func](auto&&... args) {
        static auto lastSecond = std::chrono::steady_clock::now();
        static int iterations = 0;

        func(std::forward<decltype(args)>(args)...);
        iterations++;

        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastSecond).count() >= 1) {
            std::cout << "Iterations per second: " << iterations / 1000 << "k" << '\n';
            iterations = 0;
            lastSecond = now;
        }
    };
}

void make_graphs() {
    auto seihr_system = seihr(100'000);
    auto const& reactions_seihr = seihr_system.getReactions();

    std::string filename_seihr = "seihr_graph.png";
    generate_graph(reactions_seihr, filename_seihr);

    auto circadian_system = circadian_oscillator();
    auto const& reactions_circadian = circadian_system.getReactions();

    std::string filename_circadian = "circadian_graph.png";
    generate_graph(reactions_circadian, filename_circadian);

    auto simple_system = simple();
    auto const& reactions_simple = simple_system.getReactions();

    std::string filename_simple = "simple_graph.png";
    generate_graph(reactions_simple, filename_simple);
}

void plot_circadian() {
    auto circadian_system = circadian_oscillator();
    auto trajectoryMonitor = SpeciesTrajectoryMonitor();
    auto ips_circadian = make_ips_counter(trajectoryMonitor);

    std::cout << "Simulating Circadian Rhythm..." << std::endl;
    auto s_circadian = Simulator(circadian_system, 100);
    s_circadian.simulate(ips_circadian);

    auto plot = plot_t("Trajectory of Circadian Rhythm", "Time, hours", "Count", 1920, 1080);
    for (const auto& [species, quantities] : trajectoryMonitor.getSpeciesQuantities()) {
        std::string speciesName = species.getName();

        if (speciesName == "C" || speciesName == "A" || speciesName == "R") {
            plot.lines(speciesName, trajectoryMonitor.getTimePoints(), quantities);
        }
    }

    plot.process();
    plot.save_to_png("circadian.png");
}

void plot_seihr() {
    auto seihr_system = seihr(10'000); // 589'755 for DK_NJ
    auto trajectoryMonitor = SpeciesTrajectoryMonitor();
    auto ips_seihr_species_monitor = make_ips_counter(trajectoryMonitor);

    std::cout << "Simulating SEIHR..." << std::endl;
    auto s_seihr = Simulator(seihr_system, 100);
    s_seihr.simulate(ips_seihr_species_monitor);

    auto plot = plot_t("Trajectory of SEIHR (N=10'000, rate=0.001)", "Time, days", "Count", 1920, 1080);
    for (const auto& [species, quantities] : trajectoryMonitor.getSpeciesQuantities()) {
        std::string speciesName = species.getName();

        if (speciesName == "H") {
            std::vector<double> transformedQuantities;
            std::transform(quantities.begin(), quantities.end(), std::back_inserter(transformedQuantities), [](double quantity) { return quantity * 1000; });

            plot.lines(speciesName + "*1000", trajectoryMonitor.getTimePoints(), transformedQuantities);
        } else {
            plot.lines(speciesName, trajectoryMonitor.getTimePoints(), quantities);
        }
    }

    plot.process();
    plot.save_to_png("seihr.png");
}


void plot_simple() {
    auto simple_system = simple();
    auto trajectoryMonitor = SpeciesTrajectoryMonitor();
    auto ips_species_monitor = make_ips_counter(trajectoryMonitor);

    std::cout << "Simulating Simple..." << std::endl;
    auto s_simple = Simulator(simple_system, 100'000);
    s_simple.simulate(ips_species_monitor);

    auto plot_simple = plot_t("Trajectory of Simple (A=100, B=0, C=2)", "Time", "Count", 1920, 1080);
    for (const auto& [species, quantities] : trajectoryMonitor.getSpeciesQuantities()) {
        std::string speciesName = species.getName();

        plot_simple.lines(speciesName, trajectoryMonitor.getTimePoints(), quantities);
    }

    plot_simple.process();
    plot_simple.save_to_png("simple.png");
}

#include <thread>
#include <mutex>
#include <future>
#include <numeric>

//void peak_seihr() {
//    auto seihr_system_nj = seihr(589'755); // N_NJ
//    auto seihr_system_dk = seihr(5'882'763); // N_DK
//
//    std::cout << "Simulating SEIHR..." << std::endl;
//
//    auto s_seihr_nj = Simulator(seihr_system_nj, 100);
//    auto h_mon_nj = SpeciesPeakMonitor("H");
//    auto ips_h_mon_nj = make_ips_counter(h_mon_nj);
//    s_seihr_nj.simulate(ips_h_mon_nj);
//
//    auto s_seihr_dk = Simulator(seihr_system_dk, 100);
//    auto h_mon_dk = SpeciesPeakMonitor("H");
//    auto ips_h_mon_dk = make_ips_counter(h_mon_dk);
//    s_seihr_dk.simulate(ips_h_mon_dk);
//
//    std::cout << "Peak of Hospitalized in NJ: " << h_mon_nj.getSpeciesPeak() << std::endl;
//    std::cout << "Peak of Hospitalized in DK: " << h_mon_dk.getSpeciesPeak() << std::endl;
//}

//void run_simulation(int N, int num_simulations, double& avg_peak, std::mutex& mtx) {
//    double total_peak = 0;
//
//    for (int i = 0; i < num_simulations; ++i) {
//        auto seihr_system = seihr(N);
//
//        auto s_seihr = Simulator(seihr_system, 100);
//        auto h_mon = SpeciesPeakMonitor("H");
//        auto ips_h_mon = make_ips_counter(h_mon);
//
//        s_seihr.simulate(ips_h_mon);
//
//        total_peak += h_mon.getSpeciesPeak();
//    }
//
//    mtx.lock();
//    avg_peak = total_peak / num_simulations;
//    mtx.unlock();
//}
//
//void peak_seihr(int num_simulations) {
//    const int N_NJ = 589'755;
//    const int N_DK = 5'882'763;
//
//    std::cout << "Simulating SEIHR..." << std::endl;
//
//    std::mutex mtx;
//    double avg_peak_nj = 0;
//    double avg_peak_dk = 0;
//
//    std::thread t1(run_simulation, N_NJ, num_simulations, std::ref(avg_peak_nj), std::ref(mtx));
//    std::thread t2(run_simulation, N_DK, num_simulations, std::ref(avg_peak_dk), std::ref(mtx));
//
//    t1.join();
//    t2.join();
//
//    std::cout << "Average peak of Hospitalized in NJ over " << num_simulations << " simulations: " << avg_peak_nj << std::endl;
//    std::cout << "Average peak of Hospitalized in DK over " << num_simulations << " simulations: " << avg_peak_dk << std::endl;
//}

double run_simulation(int N) {
    auto seihr_system = seihr(N);

    auto s_seihr = Simulator(seihr_system, 100);
    auto h_mon = SpeciesPeakMonitor("H");

    s_seihr.simulate(h_mon);

    return *h_mon.speciesPeak;
}

void peak_seihr(int num_simulations) {
    const int N_NJ = 589'755;
    const int N_DK = 5'882'763;

    std::cout << "Simulating SEIHR..." << std::endl;

    std::vector<std::future<double>> futures_nj;
    std::vector<std::future<double>> futures_dk;

    for(int i = 0; i < num_simulations; ++i) {
        futures_nj.push_back(std::async(std::launch::async, run_simulation, N_NJ));
        futures_dk.push_back(std::async(std::launch::async, run_simulation, N_DK));
    }

    std::vector<double> results_nj;
    std::vector<double> results_dk;

    for(auto &f : futures_nj) {
        results_nj.push_back(f.get());
    }
    for(auto &f : futures_dk) {
        results_dk.push_back(f.get());
    }

    double avg_peak_nj = std::accumulate(results_nj.begin(), results_nj.end(), 0.0) / results_nj.size();
    double avg_peak_dk = std::accumulate(results_dk.begin(), results_dk.end(), 0.0) / results_dk.size();

    std::cout << "Average peak of Hospitalized in NJ over " << num_simulations << " simulations: " << avg_peak_nj << std::endl;
    std::cout << "Average peak of Hospitalized in DK over " << num_simulations << " simulations: " << avg_peak_dk << std::endl;
}

int main(int argc, char const *argv[])
{
    //make_graphs();
    //plot_simple();
    //plot_circadian();
    //plot_seihr();
    peak_seihr(10);
}
