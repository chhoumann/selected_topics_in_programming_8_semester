#ifndef MONITOR_H
#define MONITOR_H

class Monitor {
public:
    virtual ~Monitor() = default;

    virtual void operator()(const System& system, double t) = 0;
};

#endif //MONITOR_H
