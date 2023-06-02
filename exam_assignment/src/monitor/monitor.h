#ifndef MONITOR_H
#define MONITOR_H

#include "../types.h"

class Monitor {
public:
    virtual ~Monitor() = default;

    virtual void operator()(const System& system, double t) = 0;
};

#endif //MONITOR_H
