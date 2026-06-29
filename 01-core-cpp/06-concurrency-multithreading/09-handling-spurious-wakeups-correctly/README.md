# Handling Spurious Wakeups Correctly

## Spurious wakeups

Spurious wakeups is an event where thread got wakeup when it shouldn't wakeup yet.

Example pseudocode

```c++
void consumer()
{
    cv.wait();

    cout << "Data consumed!" << endl;
}

void producer()
{
    // Simulate data production
    this_thread::sleep_for(chrono::seconds(100));

    // logging notification to console
    cout << "Data Produced!" << endl;

    cv.notify_one();
}
```

Without predicate in the condition variable, there's a chance where consumer wake up and continue to run `cout << "Data consumed!" << endl;`, we don't want this.

The reason this can happen because `cv.wait()` just make the thread yield, not waiting until notify get called.

## Lost wakeup

Lost wakeup is an event where thread cannot be woken up because there's no other thread can wake it up.

```c++
void consumer()
{
    this_thread::sleep_for(chrono::seconds(100));
    cv.wait();

    cout << "Data consumed!" << endl;
}

void producer()
{
    // logging notification to console
    cout << "Data Produced!" << endl;

    cv.notify_one();
}
```

This will make the consumer start last, but producer already do notify, that means no thread can wakeup the consumer.

## How to fix

To fix this, we need to use predicate and lock in our condition variable.

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

You see the

```c++
cv.wait(lock, [] { return data_ready; });
```

This actually equivalent to

```c++
while (predicate()) {
    cv.wait(lock);
}
```

This will prevent Spurrious wakeup and Lost wakeup to happen.

## Source

https://www.geeksforgeeks.org/cpp/cpp-multithreading-condition-variables/