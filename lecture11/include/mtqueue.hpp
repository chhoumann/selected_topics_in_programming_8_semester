#ifndef CONCURRENCY_MTQUEUE_H
#define CONCURRENCY_MTQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class mtqueue {
	std::queue<T> container; // TODO: benchmark other containers
	// TODO: add multi-threading safety (MT-safety)
    std::mutex mtx;
	std::condition_variable cv;

public:
    void put(T&& item) {
		// TODO: implement putting the element into container
		std::unique_lock<std::mutex> lock(mtx);
		container.emplace(std::move(item));
        lock.unlock();
		cv.notify_one();
    }
    void put(const T& item) {
		// TODO: implement putting the element into container
		std::unique_lock<std::mutex> lock(mtx);
		container.push(item);
		lock.unlock();
		cv.notify_one();
	}
	T get(){
		// TODO: implement fetching an element from a container
		std::unique_lock<std::mutex> lock(mtx);

		// waits for container item to be available
		cv.wait(lock, [this] { return !container.empty(); });

		// grab item, then remove it
		T item = std::move(container.front());
		container.pop();

		return item;
	}
};

#endif	// CONCURRENCY_MTQUEUE_H
