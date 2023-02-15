#include "number_series.h"
#include <iterator>
#include <iostream>
#include <random>

// Currently getting segfault here???
NumberSeries NumberSeries::MakeRandom(int upper, int lower, int length) {
    std::vector<int> series(length);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(lower, upper);

    auto gen = [&uni, &rng]()
    {
        auto x = uni(rng);
        std::cout << x << std::endl;
        return uni(rng);
    };

    std::generate(series.begin(), series.end(), gen);

    return NumberSeries{std::move(series)};
}