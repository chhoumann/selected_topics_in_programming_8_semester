#include <vector>
#include <iostream>
#include <algorithm>

class NumberSeries {

public:
    int minimum{};
    int maximum{};

    explicit NumberSeries(const std::vector<int>&& ns) {
        std::cout << "Custom constructor" << std::endl;

        series = std::move(ns);
        auto res = std::minmax_element(series.begin(), series.end());

        minimum = *res.first;
        maximum = *res.second;
    }

    ~NumberSeries() {
        series.clear();
    }

    static NumberSeries MakeRandom(int upper, int lower, int length);

    inline NumberSeries operator+(const NumberSeries& rhs);
    inline NumberSeries operator+=(const NumberSeries &rhs);
    inline bool operator<(const NumberSeries &rhs);

private:
    std::vector<int> series{};
};