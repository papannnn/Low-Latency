# Exception Propagation Across Threads

## Thread creation stack

We must know when we create another thread, for example using `std::thread`, `main` thread and the new thread actually doesn't share the same stack.

```
                Process
+--------------------------------------------------+
|                                                  |
|  Main Thread                     Worker Thread   |
|                                                  |
|  Main Stack                     Thread Stack     |
|  +-----------+                  +-----------+    |
|  |  main()   |                  | worker()  |    |
|  |-----------|                  +-----------+    |
|  |  foo()    |                        ^          |
|  +-----------+                        |          |
|                                      created by  |
|                              std::thread(worker) |
|                                                  |
+--------------------------------------------------+
```

## Stack unwinding

When exception happen, what it does it destroying the function stack until it found a try catch.

If the stack is empty and it still didn't find any try catch, it will trigger `std::terminate`.

That means, we can't just put 

```c++
try {
    thread_1.join()
} catch () {
    //...
}
```

Since thread is having different stack vs main function.

## Solution 1

We can put try catch inside thread instead

```c++
int main () {
    std::thread t([] {
        try {
            throw std::runtime_error("Error");
        } catch (const std::exception &e) {
            // ...
        }
    });

    t.join();
}
```

But this approach can't make `main` get notified when thread is throwing error.

## Solution 2

We can use `std::promise` and `std::future`

```c++
#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>

void worker(std::promise<int> promise)
{
    try
    {
        // Simulate something going wrong
        throw std::runtime_error("Something went wrong in worker");

        // If no exception:
        // promise.set_value(42);
    }
    catch (...)
    {
        // Capture and store the current exception
        promise.set_exception(std::current_exception());
    }
}

int main()
{
    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    std::thread t(worker, std::move(promise));

    try
    {
        // Either returns the value or rethrows the stored exception
        int result = future.get();

        std::cout << "Result: " << result << '\n';
    }
    catch (const std::exception& e)
    {
        std::cout << "Caught exception from worker: "
                  << e.what() << '\n';
    }

    t.join();
}
```

## Solution 3

Using `std::exception_ptr`

```c++
void worker(std::exception_ptr &ptr) {
    try {
        throw std::runtime_error("error");
    } catch (...) {
        ptr = std::current_exception();
    }
}

int main () {
    std::exception_ptr ptr;
    std::thread t(worker, std::ref(ptr));

    t.join();
    if (ptr) {
        try {
            std::rethrow_exception(ptr);
        } catch (...) {
            // ...
        }
    }
}
```

## Source

https://www.youtube.com/watch?v=Fm3dlAzEQmg