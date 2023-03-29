#include <algorithm>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
    ThreadPool(size_t num_threads) {
        workers.reserve(num_threads);
        for (size_t i = 0; i < num_threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock lock{mutex_};
                        condition_.wait(lock, [this] { return !tasks.empty() || terminate; });
                        if (terminate && tasks.empty()) {
                            break;
                        }
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::scoped_lock lock{mutex_};
            terminate = true;
        }
        condition_.notify_all();
        for (auto &worker : workers) {
            worker.join();
        }
    }

    template <typename F, typename... Args>
    auto async(F&& f, Args&&... args) {
        using return_type = std::invoke_result_t<F, Args...>;
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        auto future = task->get_future();
        {
            std::scoped_lock lock{mutex_};
            tasks.emplace([task] { (*task)(); });
        }
        condition_.notify_one();
        return future;
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool terminate{false};
};
