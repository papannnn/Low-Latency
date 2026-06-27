# Mutex Types and Proper Usage

## std::mutex

In C++, when multiple thread want to modify the same resources, it may causing race conditions. One of the way to prevent that is by using mutex.

### Creating mutex

```c++
std::mutex mutex_object_name;
```

### Lock the thread

```c++
mutex_object_name.lock()
```

### Unlock the thread

```c++
mutex_object_name.unlock()
```

### Code Without Mutex Synchronization

```c++
// C++ program to illustrate the race conditions 
#include <iostream>
#include <thread>

using namespace std;

// Shared resource
int number = 0;

// function to increment the number
void increment(){
    
    // increment number by 1 for 1000000 times
    for(int i=0; i<1000000; i++){
        number++;
    }
}

int main()
{
    // Create thread t1 to perform increment()
    thread t1(increment);
    
    // Create thread t2 to perform increment()
    thread t2(increment);
    
    // Start both threads simultaneously
    t1.join();
    t2.join();
    
    // Print the number after the execution of both threads
    cout << "Number after execution of t1 and t2 is " << number;
    
    return 0;
}
```

Output

```
Number after execution of t1 and t2 is 1058072
```

```
Number after execution of t1 and t2 is 1456656
```

```
Number after execution of t1 and t2 is 2000000
```

### Code with mutex synchronization

```c++
// C++ program to illustrate the thread synchronization using mutex
#include <iostream>
#include <thread>

using namespace std;

// import mutex from C++ standard library
#include <mutex>

// Create object for mutex
mutex mtx;

// Shared resource
int number = 0;

// function to increment the number
void increment(){
    
    // Lock the thread using lock
    mtx.lock();
    
    // increment number by 1 for 1000000 times
    for(int i=0; i<1000000; i++){
        number++;
    }
    
    // Release the lock using unlock()
    mtx.unlock();
}

int main()
{
    // Create thread t1 to perform increment()
    thread t1(increment);
    
    // Create thread t2 to perform increment()
    thread t2(increment);
    
    // Start both threads simultaneously
    t1.join();
    t2.join();
    
    // Print the number after the execution of both threads
    std::cout<<"Number after execution of t1 and t2 is "<<number;
    
    return 0;
}
```

## std::recursive_mutex

## std::timed_mutex

## std::recursive_timed_mutex

## std::shared_mutex

## std::shared_timed_mutex

## Source

https://www.geeksforgeeks.org/cpp/std-mutex-in-cpp/