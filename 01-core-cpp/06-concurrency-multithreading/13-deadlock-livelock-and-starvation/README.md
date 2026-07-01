# Deadlock, Livelock, and Starvation

## Deadlock

In order deadlock to happen, these condition must happen

### Mutual Exclusion

Only one process may use resource at a time

### Hold and Wait

Process hold resources while waiting for another

### No Preemption

Can't take resource away from a process

### Circular Wait

The waiting process form a cycle

## Example Deadlock

```c++
#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

mutex resource1;
mutex resource2;

void threadFunction1() {
    lock_guard<mutex> lock1(resource1);
    this_thread::sleep_for(chrono::milliseconds(100)); // Simulating work
    lock_guard<mutex> lock2(resource2); // This will wait for resource2

    cout << "Thread 1 has both mutexes" << endl;
}

void threadFunction2() {
    lock_guard<mutex> lock2(resource2);
    this_thread::sleep_for(chrono::milliseconds(100)); // Simulating work
    lock_guard<mutex> lock1(resource1); // This will wait for resource1

    cout << "Thread 2 has both mutexes" << endl;
}

int main() {
    thread t1(threadFunction1);
    thread t2(threadFunction2);

    t1.join();
    t2.join();

    return 0;
}
```

## Livelock

Thread continuosly react to each other but still make no progress, threads are active but unable to do any useful works.

```
Thread 1

lock(A)
if (lock(B) fail) {
    unlock(A)
    retry
}
```

```
Thread 2

lock(B)
if (lock(A) fail) {
    unlock(B)
    retry
}
```

## Starvation

When thread wait unreasonably long time because other thread always pick the resources first.

Assuming you wait in queue, when you want to go in, suddenly VIP person comes and take your place, that means you need to wait again. It happens again and again.

The solution of this can use Fair Lock that use FIFO.

## Source

https://yc-kuo.medium.com/hands-on-multithreading-with-c-03-deadlock-97c42333d8e1

https://www.youtube.com/watch?v=b8hXSy1a3hw

https://dev.to/anwaar/multithreading-concepts-part-2-starvation-1abb