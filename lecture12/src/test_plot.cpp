#include "plot.hpp"
#include <random>
#include <cmath>
#include <iostream>

int main()
{
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 10);

    // Create two indexed random number series
    std::vector<std::pair<double, double>> series1, series2;
    for (double x = 0; x < 100; x += 0.5)
    {
        double y1 = 2 * x + dis(gen);
        double y2 = std::sin(x) * 4 + 5;

        series1.emplace_back(x, y1);
        series2.emplace_back(x, y2);
    }

    std::cout << "Plotting...\n";
    std::cout << series1.size() << " points\n";

    Plot plot;

    plot.scatter(series1);
    plot.lines(series2);

    plot.process();

    return 0;

    return 0;
}
