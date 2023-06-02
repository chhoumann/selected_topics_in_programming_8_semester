#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <mutex>
#include <future>
#include <numeric>
#include <functional>
#include <memory>
#include <condition_variable>

class ThreadPool {
public:
    explicit ThreadPool(size_t concurrency_level);

    template<typename Function, typename... Args>
    auto enqueue(Function&& f, Args&&... args) -> std::future<decltype(f(args...))>;

private:
    size_t available;
    std::mutex queue_mutex;
    std::condition_variable cv_available;
};

#endif //THREADPOOL_H
