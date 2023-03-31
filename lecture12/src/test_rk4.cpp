/** Tests rk4 library */
#include "rk4.h"

#include <iostream>  // std::cout
#include <iomanip>   // std::setprecision
#include <vector>    // std::vector

int main()
{
    size_t vars = 3;
    size_t steps = 50;
    std::vector<double> t(vars * steps, 0.0);
    t[2] = 1.0;
    solve_trig(steps, 0.01, t.data());
    for (size_t v = 1; v < vars; ++v) {
        std::cout << "v" << v << ": ";
        for (size_t s = 0; s < steps; ++s)
            std::cout << std::fixed << std::setprecision(6) << "(" << t[s * vars] << "," << t[s * vars + v] << "),";
        std::cout << "\n";
    }
}
