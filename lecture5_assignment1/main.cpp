#include <iostream>
#include "number_series.h"
#include <vector>
#include <algorithm>
#include <chrono>

using namespace series;

int main()
{
    std::vector<number_series> nss{100000};

    for (size_t i = 0; i < nss.size(); ++i)
        nss[i] = number_series::MakeRandom(1, 10, 100);

    for (size_t i = 0; i < nss.size(); ++i)
        nss[i] += number_series::MakeRandom(1, 10, 100);

    std::cout << "Min: " << nss[0].get_min() << " | Max: " << nss[0].get_max() << std::endl;

    auto t_start = std::chrono::high_resolution_clock::now();
    std::sort(nss.begin(), nss.end(), [](const number_series &x, const number_series &y)
              { return x < y; });

    std::cout << "Min: " << nss[0].get_min() << " | Max: " << nss[0].get_max() << std::endl;

    auto t_stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(t_stop - t_start).count();

    std::cout << "Time to sort: " << duration << "ms." << std::endl;
    // Time to sort: 0.0161ms.

    return 0;
}