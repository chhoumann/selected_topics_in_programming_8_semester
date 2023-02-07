// Create a benchmarking utility class
// The class should help to measure the several named time points and then report the time durations in milliseconds with fractions. Use std::vector for storage and std::chrono::high_resolution_clock to measure the time.
#include <vector>
#include <chrono>
#include <string>
#include <iostream>
#include <thread>

class Benchmark
{
public:
    Benchmark() = default;

    void start(const std::string &point_name)
    {
        points.push_back({point_name,
                          std::chrono::high_resolution_clock::now()});
    }

    void stop(const std::string& point_name) {
        auto current = std::chrono::high_resolution_clock::now();
        auto previous = points.back().time;
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(current - previous);
        std::cout << point_name << " took " << duration.count() << "ms." << std::endl;
    }

    void report() {
        auto previous = points[0].time;
        
        for (const auto& point : points) {
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(point.time - previous);
            std::cout << point.name << " took " << duration.count() << "ms." << std::endl;
            previous = point.time;
        }
    }

private:
    struct TimePoint
    {
        std::string name;
        std::chrono::high_resolution_clock::time_point time;
    };

    std::vector<TimePoint> points;
};

void doWork(int duration) {
    std::chrono::milliseconds workDuration(duration);
    std::this_thread::sleep_for(workDuration);
}

int main() {
    Benchmark b;

    b.start("Point 1");
    doWork(100);
    b.stop("Point 1");

    b.start("Point 2");
    doWork(251);
    b.stop("Point 2");

    b.report();

    return 0;
}