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

It's same like mutex, but same thread can lock one mutex multiple times using recursive mutex.

If thread `T1` first call `lock()` on recursive mutex `m1`. That means, `T1` can do `lock()` again and again many times.

But if `T1` lock it 10x, that means `T1` also need to unlock it 10x to make other thread can access it.

## std::timed_mutex

This mutex is blocked till timeout time or lock is acquired and return true if success. Otherwise, false.

The member function has `try_lock_for` and `try_lock_until`

## std::recursive_timed_mutex

It's just a combination of `std::recursive_mutex` and `std::timed_mutex`.

## std::shared_mutex

In some cases, several threads need to read the data from shared resources at the same time.

We can use `std::shared_mutex`

### Type of lock in `std::shared_mutex`

#### Unique lock

It is a unique lock that can only be acquired by a single thread at a time.

You will use `lock()` & `unlock()`

#### Shared lock

The shared lock is a non-exclusive lock that several threads can acquire at once.

You will use `lock_shared()` & `unlock_shared()`

## std::shared_timed_mutex

It's just a combination of `std::shared_mutex` and `std::timed_mutex`.

## Source

### `std::mutex`
https://www.geeksforgeeks.org/cpp/std-mutex-in-cpp/

### `std::recursive_mutex`

https://www.youtube.com/watch?v=yCYU2k77E4A

### `std::timed_mutex`

https://www.youtube.com/watch?v=x0sHzDyETUc

### `std::shared_mutex`

https://www.geeksforgeeks.org/cpp/std-shared_mutex-in-cpp/
