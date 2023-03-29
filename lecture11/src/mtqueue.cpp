#include "mtqueue.hpp"
#include "threadpool.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <random>

constexpr size_t NUM_PRODUCERS = 5;
constexpr size_t NUM_CONSUMERS = 5;
constexpr size_t NUM_ITEMS = 100;

void produce(mtqueue<int>& q)
{
    // TODO: create producer routine: put random numbers into queue
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);

    for (size_t i = 0; i < NUM_ITEMS; ++i) {
        q.put(dis(gen));
    }
}

void consume(mtqueue<int>& q)
{
    // TODO: create consumer routine: fetch and print values
    for (size_t i = 0; i < NUM_ITEMS; ++i) {
        int num = q.get();
        std::cout << "Consumed: " << num << std::endl;
    }
}

int main()
{
    // TODO: create an instance of a queue
    mtqueue<int> q;

    // TODO: spawn multiple threads for producer and consumer using the queue
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (size_t i = 0; i < NUM_PRODUCERS; ++i) {
        producers.emplace_back(std::thread(produce, std::ref(q)));
    }

    for (size_t i = 0; i < NUM_CONSUMERS; ++i) {
        consumers.emplace_back(std::thread(consume, std::ref(q)));
    }

    for (auto& t : producers) {
        t.join();
    }

    for (auto& t : consumers) {
        t.join();
    }

    // NOTE: producer and consumer expect to bind to the queue by reference:
    // 1) STL uses values, therefore plain queue argument might not work: wrap the queue into std::ref instead.
    // 2) Using std::ref is not normal: it is a red-flag that we are doing something wrong.
    //    In this case we are sharing the queue between threads -- dangerous design!
    //    But in this exercise this is how we stress-test our queue in multi-threaded (MT) environment.
    // NOTE: many successful test runs do not guarantee that the queue is MT-safe.
    // You may make your producers to work longer, try running optimized builds hoping to trigger an error,
    // or enable option(TSAN "Thread Sanitizer" ON) in the CMakeLists.txt
    // (remember to regenerate the cmake build by deleting the build directories).
    // Limitations:
    //  - clang++-13 does not ship std::jthread.
    //  - thread sanitizer is supported only on Linux and MacOS.

    std::cout << "Done" << std::endl;

    // --- THREADPOOL EXERCISE ---
    ThreadPool pool(std::thread::hardware_concurrency());

    for (int i = 0; i < 5; ++i) {
        pool.async(produce, std::ref(q));
        pool.async(consume, std::ref(q));
    }

    return 0;
}