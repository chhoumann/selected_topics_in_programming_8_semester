#include "number_series.h"
#include "series_wrapper.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace series;

void run();
void run_wrapper();

constexpr int size = 100000;

int main()
{
    run();

    run_wrapper();

    return 0;
}

void run()
{
    std::vector<number_series> nss(size);

    for (size_t i = 0; i < size; ++i)
        nss[i] = number_series::make_random(1, 10, 100);

    for (size_t i = 0; i < size; ++i)
        nss[i] += number_series::make_random(1, 10, 100);

    auto t_start = std::chrono::high_resolution_clock::now();
    // Might not be necessary to use the third argument (lambda) here, as sort should use < operator by default.
    std::sort(nss.begin(), nss.end(), [](const number_series &x, const number_series &y)
              { return x < y; });

    auto t_stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(t_stop - t_start).count();

    std::cout << "Time to sort: \t\t" << duration << "ms." << std::endl;
}

void run_wrapper()
{
    std::vector<series_wrapper> nss(size);

    for (size_t i = 0; i < size; ++i) {
        nss.push_back(series_wrapper::make_random(1, 10, 100));
    }

    // Prefer ranged loops
    for (size_t i = 0; i < size; ++i)
    {
        auto random = series_wrapper::make_random(1, 10, 100);
        nss.at(i) += random; // Wouldn't need to do this .at thing if I used ranged loop. + overhead here of checking index range.
    }
    auto t_start = std::chrono::high_resolution_clock::now();
    // See 'run' fn on sort
    std::sort(nss.begin(), nss.end(), [](const series_wrapper &x, const series_wrapper &y)
              { return x < y; });

    auto t_stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(t_stop - t_start).count();

    std::cout << "Time to sort wrapper: \t" << duration << "ms." << std::endl;
}

// Overall OK. Should have included performance metrics in hand-in. Also before and after padding (?). What were my conclusions?