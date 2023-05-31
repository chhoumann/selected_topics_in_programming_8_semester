#include <thread>
#include <mutex>
#include <future>
#include <numeric>
#include <functional>
#include <memory>
#include <condition_variable>

class ThreadPool {
public:
    explicit ThreadPool(size_t concurrency_level)
            : available(concurrency_level) {
    }

    template<typename Function, typename... Args>
    auto enqueue(Function&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using ReturnType = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<ReturnType()> >(
                std::bind_front(std::forward<Function>(f), std::forward<Args>(args)...)
        );

        std::future<ReturnType> result = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            // Wait until there's an available slot for a new thread.
            cv_available.wait(lock, [this]{ return available > 0; });

            --available;
        }

        std::jthread([this, task]() {
            (*task)();

            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                ++available;
            }

            cv_available.notify_one();
        }).detach();

        return result;
    }

private:
    size_t available;
    std::mutex queue_mutex;
    std::condition_variable cv_available;
};
