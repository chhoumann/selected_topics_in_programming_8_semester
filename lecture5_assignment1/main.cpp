#include "number_series.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace series;

void run();
void run_wrapper();

int main()
{
    run();

    // run_wrapper();

    return 0;
}

void run()
{
    std::vector<number_series> nss(100000);

    for (size_t i = 0; i < nss.size(); ++i)
        nss[i] = number_series::make_random(1, 10, 100);

    for (size_t i = 0; i < nss.size(); ++i)
        nss[i] += number_series::make_random(1, 10, 100);

    std::cout << "Min: " << nss[0].get_min() << " | Max: " << nss[0].get_max() << std::endl;

    auto t_start = std::chrono::high_resolution_clock::now();
    std::sort(nss.begin(), nss.end(), [](const number_series &x, const number_series &y)
              { return x < y; });

    std::cout << "Min: " << nss[0].get_min() << " | Max: " << nss[0].get_max() << std::endl;

    auto t_stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(t_stop - t_start).count();

    std::cout << "Time to sort: " << duration << "ms." << std::endl;
    // Time to sort: 36.5488ms.
}

// void run_wrapper()
// {
//     std::vector<series_wrapper> nss(100000);

//     for (size_t i = 0; i < nss.size(); ++i)
//         nss[i] = series_wrapper::make_random(1, 10, 100);

//     for (size_t i = 0; i < nss.size(); ++i)
//         nss[i] += series_wrapper::make_random(1, 10, 100);

//     std::cout << "Min: " << nss[0].get_min() << " | Max: " << nss[0].get_max() << std::endl;

//     auto t_start = std::chrono::high_resolution_clock::now();
//     std::sort(nss.begin(), nss.end(), [](const number_series &x, const number_series &y)
//               { return x < y; });

//     std::cout << "Min: " << nss[0].get_min() << " | Max: " << nss[0].get_max() << std::endl;

//     auto t_stop = std::chrono::high_resolution_clock::now();
//     auto duration = std::chrono::duration<double, std::milli>(t_stop - t_start).count();

//     std::cout << "Time to sort wrapper: " << duration << "ms." << std::endl;
//     // Time to sort: 36.5488ms.
// }