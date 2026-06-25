# Thread Creation, Join, Detach, Lifecycle Management

## Create a thread

To create a thread, you just need to do

```
thread thread_name(callable);
```

Example code

```c++
#include <bits/stdc++.h>
using namespace std;

// Function to be run by the thread
void func() {
    cout << "Hello from the thread!" << endl;
}

int main() {
    
    // Create a thread that runs 
    // the function func
    thread t(func);
    
    // Main thread waits for 't' to finish
    t.join();  
    cout << "Main thread finished.";
    return 0;
}
```

Output
```
Hello from the thread!
Main thread finished.
```

## Joining a thread

`join()` is used to wait a thread to finish an execution.

Before calling join, please check if thread is joinable

If join() is called on a non-joinable thread, it throws a std::system_error exception.

## Detaching a thread

A joinable thread can be detached by using `detach()`

## Example code

```c++
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

void task1() {
    cout << "Thread 1 is running. ID: " << this_thread::get_id() << "\n";
}

void task2() {
    cout << "Thread 2 is running. ID: " << this_thread::get_id() << "\n";
}

int main() {
    thread t1(task1);
    thread t2(task2);

    // Get thread IDs
    cout << "t1 ID: " << t1.get_id() << "\n";
    cout << "t2 ID: " << t2.get_id() << "\n";

    // Join t1 if joinable
    if (t1.joinable()) {
        t1.join();
        cout << "t1 joined\n";
    }

    // Detach t2 if joinable
    if (t2.joinable()) {
        t2.detach();
        cout << "t2 detached\n";
    }

    // Give detached thread time to complete
    this_thread::sleep_for(chrono::milliseconds(100));

    cout << "Main thread finished.\n";
    return 0;
}
```

Output

```
Thread 1 is running. ID: Thread 2 is running. ID: 140737213290176
140737347512000
t1 ID: 140737347512000
t2 ID: 140737213290176
t1 joined
t2 detached
Main thread finished.
```

## Callable in multithreading

### Function pointer

```c++
#include <bits/stdc++.h>
using namespace std;

// Function to be run 
// by the thread
void func(int n) {
    cout << n;
}

int main() {
    
    // Create a thread that runs 
    // the function func
    thread t(func, 4);
    
    // Wait for thread to finish
    t.join();
    return 0;
}
```

### Lambda expression

```c++
#include <iostream>
#include <thread>

using namespace std;

int main() {
    int n = 3;
    
    // Create a thread that runs 
    // a lambda expression
    thread t([](int n){
        cout << n;
    }, n);

    // Wait for the thread to complete
    t.join();
    return 0;
}
```

### Functor

```c++
#include <iostream>
#include <thread>
using namespace std;

// Define a function object (functor)
class SumFunctor {
public:
    int n;
    SumFunctor(int a) : n(a) {}

    // Overload the operator() to 
    // make it callable
    void operator()() const {
        cout << n;
    }
};

int main() {

    // Create a thread using 
    // the functor object
    thread t(SumFunctor(3));

    // Wait for the thread to 
    // complete
    t.join();
    return 0;
}   
```

### Static member function

```c++
#include <iostream>
#include <thread>

using namespace std;

class MyClass {
public:
    // Non-static member function
    void f1(int num) {
        cout << num << endl;
    }

    // Static member function that takes one parameter
    static void f2(int num) {
        cout << num;
    }
};

int main() {
    
    // Member functions 
    // requires an object
    MyClass obj;
    
    // Passing object and parameter
    thread t1(&MyClass::f1, &obj, 3);
    
    t1.join(); 
    
    // Static member function can 
    // be called without an object
    thread t2(&MyClass::f2, 7);
    
    // Wait for the thread to finish
    t2.join();  

    return 0;
}
```

## Source

https://www.geeksforgeeks.org/cpp/multithreading-in-cpp/