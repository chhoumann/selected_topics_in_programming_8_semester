#include <iostream>

struct point
{
    double x{0};
    double y{0};

    point(double x, double y) : x{x}, y{y} {};

    friend std::ostream &operator<<(std::ostream &os, const point &p);
};