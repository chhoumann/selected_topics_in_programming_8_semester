/**
 * Euler integration library implementation with exported C API.
 * See https://en.wikipedia.org/wiki/Euler_method
 */

#include "rk4.h"

#include <cstring>  // memset
#include <cstdlib>  // malloc
#include <cassert>

/** Derivative function type: given a state, compute the derivatives  */
using derive_fn = void (*)(size_t var_count, const double* state, double* derivatives);
// typedef void (*derive_fn)(size_t var_count, const double* state, double* derivatives);

/**
 * Computes the next integration step for all variables.
 * `static` means used only locally in this translation unit, i.e. do not expose this symbol.
 */
static void next(size_t var_count, double dt, derive_fn dF, double* val1, double* val2)
{
    assert(var_count > 0);
    assert(dt > 0);
    assert(dF);
    assert(val1);
    assert(val2);
    dF(var_count, val1, val2);  // compute all derivatives
    for (size_t i = 0; i < var_count; ++i)
        val2[i] = val2[i] * dt + val1[i];  // "integrate" with step dt
}

static void solve_euler(size_t var_count, size_t steps, double dt, derive_fn dF, double* t)
{
    assert(var_count > 0);
    assert(steps > 0);
    assert(dt > 0);
    assert(dF);
    assert(t);
    for (size_t i = 0; i < steps - 1; ++i)
        next(var_count, dt, dF, &t[var_count * i], &t[var_count * (i + 1)]);
}

/** Computes state change/derivative ds at a given state s. */
static void derive_trig(size_t vars, const double* s, double* ds)
{
    assert(vars >= 3);
    assert(s);
    assert(ds);
    ds[0] = 1;      // time'(t) = 1
    ds[1] = s[2];   // sin'(t) = cos(t)
    ds[2] = -s[1];  // cos'(t) = -sin(t)
}

extern "C" {  // specify C linking (no C++ name mangling, we use only functions)

// RK4_EXPORT tells the compiler to expose this symbol.
RK4_EXPORT
void solve_trig(size_t steps, double dt, double* t)
{
    assert(steps > 0);
    assert(dt > 0);
    assert(t);
    solve_euler(3, steps, dt, derive_trig, t);
}

}  // extern "C"