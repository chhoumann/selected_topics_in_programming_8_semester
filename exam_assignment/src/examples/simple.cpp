#include "examples.h"

System simple() {
    auto v = System{};

    auto A = v("A", 100);
    auto B = v("B", 0);
    auto C = v("C", 2);

    auto rate = 0.001;

    v(A + C >>= B + C, rate);

    return v;
}