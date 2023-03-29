#include <vector>
#include <future>
#include <iostream>
#include <random>
#include <chrono>

std::vector<int> produce(size_t count){
	// TODO: generate random numbers
    std::vector<int> numbers(count);

	std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> distribution(0, 100);

	for (auto& number : numbers) {
		number = distribution(generator);
	}

	return numbers;
}

int summarize(std::vector<std::future<std::vector<int>>>&& futures){
	// TODO: compute and return the sum of elements
	int sum = 0;

	for (auto& future : futures) {
		auto numbers = future.get();
		for (auto& number : numbers) {
			sum += number;
		}
	}

    return sum;
}

int main(){
	// TODO: spawn several producers
	size_t num_producers = 5;
	size_t num_per_producer = 10;

	std::vector<std::future<std::vector<int>>> futures;
	for (size_t i = 0; i < num_producers; ++i) {
		futures.push_back(std::async(std::launch::async, produce, num_per_producer));
	}

	// TODO: collect the futures, compute and announce summary
	int summary = summarize(std::move(futures));

	std::cout << "Summary: " << summary << std::endl;

    return 0;
}