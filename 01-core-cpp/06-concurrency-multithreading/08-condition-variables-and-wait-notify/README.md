# Condition Variables and Wait/Notify

## `std::condition_variable`

Is a syncronization primitive to help us do waiting until specific condition is reached.

### `wait()`

It tell the current thread to wait until got notified again by another thread

### `wait_for()`

It tell the current thread to wait until X amount of time, or got notified by another thread

### `wait_until()`

It tell the current thread to wait until it reached time X, or got notified by another thread

### `notify_one()`

This thread notify 1 thread to be wakeup, the choosing is random

### `notify_all()`

It notify all of the thread the sleep using the same `std::condition_variable`

## Example

```c++
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

// mutex to block threads
mutex mtx;
condition_variable cv;

// function to avoid spurios wakeup
bool data_ready = false;

// producer function working as sender
void producer()
{
    // Simulate data production
    this_thread::sleep_for(chrono::seconds(2));

    // lock release
    lock_guard<mutex> lock(mtx);

    // variable to avoid spurious wakeup
    data_ready = true;

    // logging notification to console
    cout << "Data Produced!" << endl;

    // notify consumer when done
    cv.notify_one();
}

// consumer that will consume what producer has produced
// working as reciever
void consumer()
{
    // locking
    unique_lock<mutex> lock(mtx);

    // waiting
    cv.wait(lock, [] { return data_ready; });

    cout << "Data consumed!" << endl;
}

// drive code
int main()
{
    thread consumer_thread(consumer);
    thread producer_thread(producer);

    consumer_thread.join();
    producer_thread.join();

        return 0;
}
```

## Source

https://www.geeksforgeeks.org/cpp/cpp-multithreading-condition-variables/