# Futures, Promises, and Async Tasks

## `std::async`

Imagine you're building an application that fetch API, if you fetch an API in your main thread, that means your whole app need to wait the API finished fetching, that's when you need `std::async`.

```c++
#include <iostream>
#include <future>

int square(int x) {
    return x * x;
}

int main () {
    std::future<int> asyncFunc = std::async(&future, 12);

    for (int i = 0; i < 10; i++) {
        std::cout << "Hello" << std::endl;
    }

    int result = asyncFunc.get();

    std::cout << result << std::endl;
}
```

## `std::future`

`std::future` is just giving you a "box" that help `std::async` / `std::promise` to store it's result.

If you want to get the result, just use `get()`. It will wait until the thread is finished processing the value.

`get()` only can be called once, it will be error if you call it >1x.

## `std::promise`

`std::promise` almost the same like `std::async`, but I don't really like it. You kinda do it manually.

```c++
// C++ program to use std::promise and std::future to
// communicate between threads.
#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace std;

// Function to perform some computation and set the result
// in a promise
void RetrieveValue(promise<int>& result)
{
    try {
        int ans = 21095022;

        // Set the result in the promise
        result.set_value(ans);
    }
    catch (...) {
        // if an error occurs set the exception
        result.set_exception(current_exception());
    }
}

int main()
{
    // Step 1: Creating a std::promise object
    promise<int> myPromise;

    // Step 2: Associate a std::future with the promise
    future<int> myFuture = myPromise.get_future();

    // Step 3: Launching a thread to perform computation and
    // set the result in the promise
    thread computationThread(RetrieveValue, ref(myPromise));

    // Step 4: Retrieve the value or handle the exception in
    // the original thread
    try {
        int result = myFuture.get();
        cout << "Result: " << result << endl;
    }
    catch (const exception& e) {
        cerr << "Exception is: " << e.what() << endl;
    }

    // thread finishes
    computationThread.join();

    return 0;
}
```

## Launch Policies

If you don't specify the launch policy, the launch policy will be
```c++
std::launch::async | std::launch::deferred
```

### std::launch::async

the task is executed on a different thread, potentially by creating and launching it first

### std::launch::deferred

the task is executed on the calling thread the first time its result is requested (lazy evaluation)

## Source

https://www.youtube.com/watch?v=4twJD5ezkag

https://www.geeksforgeeks.org/cpp/std-future-in-cpp/

https://www.geeksforgeeks.org/cpp/std-promise-in-cpp/

https://en.cppreference.com/cpp/thread/launch