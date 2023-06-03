#ifndef MONITOR_H
#define MONITOR_H

#include "../types.h"

// Part-solution to requirement 7: Implement a generic support for the state monitor in the stochastic simulation algorithm.
class Monitor {
public:
    virtual ~Monitor() = default;

    virtual void operator()(const System& system, double t) = 0;
};

#endif //MONITOR_H
