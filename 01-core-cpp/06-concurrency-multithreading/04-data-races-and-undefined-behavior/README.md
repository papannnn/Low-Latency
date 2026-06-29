# Data Races and Undefined Behavior

## Data Races

Data Races is commonly happened in concurrent programming.

It occurs when >1 thread accessing same data source and at least one is writing the data source.

Data Races can lead to undefined behavior. That means program can do unpredictable behavior.

## Example data races

```c++
// C++ Program to illustrate the data race condition
#include <iostream>
#include <thread>
using namespace std;

// Global counter variable
int counter = 0;

// Function to increment the counter
void increment()
{
    for (int i = 0; i < 100000; ++i) {
        ++counter;
    }
}

int main()
{
    // Create two threads that run the increment function
    // concurrently
    thread t1(increment);
    thread t2(increment);

    // Wait for both threads to finish
    t1.join();
    t2.join();

    // Print the final counter value
    cout << "Counter value: " << counter << endl;
    return 0;
}
```

```
Counter value: 146377
```

```
Counter value: 200000
```

## How to avoid data races

### Mutex

```c++
// C++ Program to demonstrate how to use mutex for thread
// synchronization
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

// Define a mutex to protect the shared counter
mutex mtx;
int counter = 0;

// Function to increment the counter
void incrementCounter()
{
    // locking mutex using lock_guard
    lock_guard<mutex> lock(mtx);
    for (int i = 0; i < 100000; ++i) {
        ++counter;
    }

    // mutex unlocked after going out of scope
}

int main()
{
    // Create two threads that call incrementCounter
    thread t1(incrementCounter);
    thread t2(incrementCounter);

    // Wait for both threads to finish
    t1.join();
    t2.join();

    // Print the final counter value
    cout << "Counter: " << counter << endl;
    return 0;
}
```

### Atomic Operation

```c++
// C++ program to illustrate how to aviod the data race
// using atomics
#include <atomic>
#include <iostream>
#include <thread>

using namespace std;

// Define an atomic integer for the counter
atomic<int> counter(0);

// Function to increment the counter
void incrementCounter()
{
    for (int i = 0; i < 100000; ++i) {
        ++counter;
    }
}

int main()
{
    // Create two threads that call incrementCounter
    thread t1(incrementCounter);
    thread t2(incrementCounter);

    // Wait for both threads to finish
    t1.join();
    t2.join();

    // Print the final counter value
    cout << "Counter: " << counter << endl;
    return 0;
}
```

### Condition Variable

```c++
// C++ Program to illustrate how to use condition variable
// to avoid the data race
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

mutex mtx;
// Condition variable for synchronization
condition_variable cv;
// Flag indicating whether work is ready
bool ready = false;

void wait_for_work()
{
    unique_lock<mutex> lock(mtx);
    // Wait until ready is true
    cout << "Start Waiting" << endl;
    cv.wait(lock, [] { return ready; });
    // resuming the work here
    cout << "Resuming the work after notification" << endl;
}

// Function to set ready flag
void set_ready()
{
    lock_guard<mutex> lock(mtx);
    // Mark work as ready
    ready = true;
    // Notify waiting threads
    cout << "Notification Sent from the other thread"
         << endl;
    cv.notify_one();
}

int main()
{
    thread worker(wait_for_work);
    thread producer(set_ready);

    // Wait for the worker thread to finish
    worker.join();
    producer.join();

    return 0;
}
```

## Source

https://www.geeksforgeeks.org/cpp/data-races-in-cpp/