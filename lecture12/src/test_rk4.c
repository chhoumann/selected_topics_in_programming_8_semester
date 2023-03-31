/** Tests rk4 library */
#include "rk4.h"

#include <stdio.h>   // printf
#include <string.h>  // memset
#include <stdlib.h>  // malloc

int main()
{
    size_t vars = 3;
    size_t steps = 50;
    double* t = (double*)malloc(vars * steps * sizeof(double));
    memset(t, 0, vars * steps * sizeof(double));
    t[2] = 1.0;
    solve_trig(steps, 0.01, t);
    for (size_t v = 1; v < vars; ++v) {
        printf("v%zd: ", v);
        for (size_t s = 0; s < steps; ++s)
            printf("(%f,%f),", t[s * vars], t[s * vars + v]);
        printf("\n");
    }
    free(t);
}
