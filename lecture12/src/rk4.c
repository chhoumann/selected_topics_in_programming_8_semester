/**
 * Runge-Kutta-4 implementation for one variable with python bindings
 */

#include "rk4.h"

#include <stddef.h>  // size_t
#include <math.h>
#include <assert.h>

/** Computes x'(t) for a given system state */
static double x_derivative(double t, double x) { return 10.0 - x; }

/** Computes next x after dt for a given system state */
static double x_next(double dt, double t, double x)
{  // Runge-Kutta-4 for one variable
    double k1 = dt * x_derivative(t, x), k2 = dt * x_derivative(t + dt / 2, x + k1 / 2),
           k3 = dt * x_derivative(t + dt / 2, x + k2 / 2), k4 = dt * x_derivative(t + dt, x + k3);
    return x + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
}

RK4_EXPORT
void solve1(size_t steps, double dt, double* t, double* x)
{
    assert(steps > 0);
    for (size_t i = 1; i < steps; ++i) {
        t[i] = dt + i * t[0];
        x[i] = x_next(dt, t[i - 1], x[i - 1]);
    }
}
