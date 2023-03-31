#ifndef PLOT_HPP
#define PLOT_HPP

#include <vector>
#include <utility>
#include <memory>

class Plot
{
public:
    Plot();
    ~Plot();

    void scatter(const std::vector<std::pair<double, double>>& data);
    void lines(const std::vector<std::pair<double, double>>& data);
    void process();

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};

#endif // PLOT_HPP
