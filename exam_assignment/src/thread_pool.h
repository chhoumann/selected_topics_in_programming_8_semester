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
    explicit ThreadPool(size_t concurrency_level) : available(concurrency_level) {};

    // Template function which takes fn `f` and parameter pack `args`. You can add tasks here.
    template<typename Function, typename... Args>
    auto enqueue(Function&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using ReturnType = decltype(f(args...));

        // Tasks are packaged into a `std::packaged_task`, allowing us to store a future return value.
        auto task = std::make_shared<std::packaged_task<ReturnType()> >(
                // `std::bind_front` is used to bind the arguments to the function `f` and return a new function object.
                std::bind_front(std::forward<Function>(f), std::forward<Args>(args)...)
        );

        // `std::future` is used to store the return value of packaged task. This is so the caller of `enqueue` can get the return value once it's ready.
        std::future<ReturnType> result = task->get_future();

        // Mutex `queue_mutex` is used with condition_variable `cv_available` to manage access to thread pool
        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            // Wait until there's an available slot for a new thread.
            // Causes thread to block if there are no available slots. Unblocked once a thread becomes available, signalled by `cv_available.notify_one()`.
            cv_available.wait(lock, [this]{ return available > 0; });

            --available;
        }

        // When a thread becomes available, the packaged task is run on a new jthread, and the number of available thread slots is incremented. jthread automatically joins when destructed, but here is immediately detached, so it can run asynchronously (independently).
        std::jthread([this, task]() {
            (*task)();

            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                ++available;
            }

            // Once we're done executing, we notify the condition variable that a thread is available.
            cv_available.notify_one();
        }).detach();

        return result;
    }

private:
    size_t available;
    std::mutex queue_mutex;
    std::condition_variable cv_available;
};

#endif //THREADPOOL_H
