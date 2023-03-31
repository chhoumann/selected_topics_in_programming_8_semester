/**
 * Runge-Kutta-4 library implementation with python bindings
 */

#include "rk4.h"

#include <stddef.h>  // size_t
#include <assert.h>

static double x_derivative(double t, double x, double y) { return y; }

static double y_derivative(double t, double x, double y)
{
    static const double radius = 1.0;
    double res = -10.0;
    if (x < radius) {
        res += (radius - x) * 300;
    }
    return res;
}

static double x_next(double dt, double t, double x, double y)
{
    double k1 = dt * x_derivative(t, x, y), k2 = dt * x_derivative(t + dt / 2, x + k1 / 2, y),
           k3 = dt * x_derivative(t + dt / 2, x + k2 / 2, y), k4 = dt * x_derivative(t + dt, x + k3, y);
    return x + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
}

static double y_next(double dt, double t, double x, double y)
{
    double k1 = dt * y_derivative(t, x, y), k2 = dt * y_derivative(t + dt / 2, x, y + k1 / 2),
           k3 = dt * y_derivative(t + dt / 2, x, x + k2 / 2), k4 = dt * y_derivative(t + dt, x, y + k3);
    return y + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
}

RK4_EXPORT
void solve2_3(size_t steps, double dt, double* t, double* x, double* y)
{
    assert(steps > 0);
    for (size_t i = 1; i < steps; ++i) {
        t[i] = i * dt + t[0];
        x[i] = x_next(dt, t[i - 1], x[i - 1], y[i - 1]);
        y[i] = y_next(dt, t[i - 1], x[i - 1], y[i - 1]);
    }
}
