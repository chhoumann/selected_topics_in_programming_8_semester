// Create a minimal class Base with a single integer field and two methods:
// A pure virtual increment() and a non-virtual decrement().
// Create a derived class Derived that just overrides the increment() method.
// Create a vector of 10 milion (smart) pointers to Base and fill it with pointers to the Derived object.
// Then measure time of
// 1. Calling increment() on all the elements in the vector
// 2. Calling decrement() on all the elements.
// 3. Finally, create another vector of 10 million Derived objects (not pointers to them).
//    Measure time of calling one of the methods on all the elements of the vector.
// Make sure you measure the _release_ build.
// What can you conclude?
#include <vector>
#include <memory>
#include <iostream>
#include <chrono>

class Base {
    public:
        virtual void increment() = 0;

        void decrement() {
            --value;
        }

    protected:
        int value = 0;
};

class Derived : public Base {
    public:
        void increment() override {
            ++Base::value;
        }
};

void measure_time_for_pt1() {
    std::vector<std::unique_ptr<Base>> bases;
    for (int i = 0; i < 10'000'000; ++i) {
        bases.push_back(std::make_unique<Derived>());
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (auto& base : bases) {
        base->increment();
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Duration for increment: " << duration.count() << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (auto& base : bases) {
        base->decrement();
    }

    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << "Duration for decrement: " << duration.count() << " ms" << std::endl;
}

void measure_time_for_pt2() {
    std::cout << "Testing derived..." << std::endl;

    std::vector<std::unique_ptr<Derived>> derived;

    for (int i = 0; i < 10'000'000; ++i) {
        derived.push_back(std::make_unique<Derived>());
    }

    auto start = std::chrono::high_resolution_clock::now();
    for (auto& base : derived) {
        base->increment();
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Duration for increment: " << duration.count() << " ms" << std::endl;

}

int main() {
    measure_time_for_pt1();
    // Duration for increment: 224 ms
    // Duration for decrement: 223 ms

    measure_time_for_pt2();
    // Testing derived...
    // Duration for increment: 217 ms
}
