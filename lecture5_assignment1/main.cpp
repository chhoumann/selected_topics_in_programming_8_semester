#include <iostream>
#include "number_series.h"

int main() {
    auto ns = NumberSeries::MakeRandom(1, 6, 1000);

    std::cout << ns.minimum << std::endl;
    std::cout << ns.maximum << std::endl;

    return 0;
}