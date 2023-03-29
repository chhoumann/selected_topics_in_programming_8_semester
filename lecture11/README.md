# Concurrency Exercises

## Thread-Protected Queue

Implement a queue for producer-consumer (see `TODO` in [mtqueue.hpp](include/mtqueue.hpp) and [mtqueue.cpp](src/mtqueue.cpp)):

1. Create a shared container of messages.
2. Create a producer periodically adding messages to the container.
3. Create a consumer printing the messages to `std::cout`.
4. Create a multiple producers.
5. Verify the code against concurrency issues
   - Linux/macOS: consult with `ThreadSanitizer` (add `-DTSAN=ON` to `cmake` configuration step).
   - Windows/MSVC: try [AppVerifier](https://learn.microsoft.com/en-us/windows-hardware/drivers/devtest/application-verifier)
6. Fix the code to ensure thread-safety.

## Futures and Promises

Implement `TODO` requirements in [future.cpp](src/futures.cpp).

## Thread Pool

Implement a thread pool class template to pre-allocate thread resources.

1. Constructor creates a number of threads waiting for tasks.
2. Use container (e.g. `std::queue`) of `std::function<void()>` to store tasks.
3. Use `std::mutex` and `std::condition_variable` to protect the task queue.
4. Destructor should notify all waiting threads to terminate and joins them.
5. Provide `async` method template similar to `std::async` to add tasks.
   - Use `std::tuple` or perfect capture to store function and arguments into lambda.
   - Use `std::promise` to return the value to a `std::future`.
   - Use `std::shared_ptr` to capture `std::promise` into lambda.
6. Test the thread pool with producers and consumers from the "Protected Queue" exercise above.

