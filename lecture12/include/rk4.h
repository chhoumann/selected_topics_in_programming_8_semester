#ifndef RK4_H
#define RK4_H

// Configure the symbol export/import (generated by cmake):
#include "rk4_export.h"

#include <stddef.h>  // size_t

#if __cplusplus
extern "C" {  // specify C linking (no C++ name-mangling)
#endif        // __cplusplus

/**
 * C API: integrates Ordinary Differential Equations describing sin and cos.
 * @param steps the number of steps
 * @param dt the step size
 * @param t data array with
 */
RK4_EXPORT
void solve_trig(size_t steps, double dt, double* t);

RK4_EXPORT
void solve1(size_t steps, double dt, double* t, double* x);

RK4_EXPORT
void solve2_2(size_t steps, double dt, double* t, double* x, double* y);

RK4_EXPORT
void solve2_3(size_t steps, double dt, double* t, double* x, double* y);

#if __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif /* RK4_H */
