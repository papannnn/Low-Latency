# C++ Memory Model

These topics are the backbone of lock-free programming.

## `std::atomic<T>` vs `volatile`

`std::atomic<T>` helps prevent data races by ensuring that operations on shared variables are performed atomically.

`volatile` keyword in C++ is used to declare variables that can be modified by external factors outside the program's control. Preventing certain optimizations that could lead to unexpected behavior.

## C++ Memory Orderings

C++11 introduced a portable memory model with six orderings

### Sequential Consistency `memory_order_seq_cst`

The default, provides a single total order of all seq_cst operations across all threads

```c++
std::atomic<int> x{0};
x.store(1); // Default is seq_cst
int val = x.load();
```

Sequential consistency guarantees that all threads see operations in the same order, with no reordering with any other operation, full memory barrier.

Cost: 15-20 cycles on x86 due to mfence.

### Acquire - Release `memory_order_acquire` / `memory_order_release`

Creates synchronization between a release store and an acquire load:

```c++
std::atomic<bool> ready{false};
int data = 0;

void producer() {
  data = 42;
  ready.store(true, std::memory_order_release);
}

void consumer() {
  while (!ready.load(std::memory_order_acquire)) {}
  assert(data == 42);
}
```

The release store "release" all prior writes. The acquire load "acquires" those writes. Together they create a happen-before relationship.

Cost: Zero extra cycles on x86. ~10 cycles on ARM.

### Relaxed `memory_order_relaxed`

Only guarantees atomicity, no ordering.

```c++
std::atomic<int> counter{0};

void increment() {
  // Only need atomicity, not ordering
  counter.fetch_add(1, std::memory_order_relaxed);
}
```

Relaxed ordering is appropriate for counters where order doesn't matter, statistics gathering, and reference counts.

Cost: Same as non atomic access.

### Acquire Release Combined `memory_order_acq_rel`

For read-modify-write operation that need both

```c++
std::atomic<int> lock{0};

void acquire_lock() {
  while (lock.exchange(1, std::memory_order_acq_rel) == 1) {}
}
```

### Consume `memory_order_consumer`

Deprecated, don't use it

## Data races vs race conditions

A data race occurs when two or more threads access the same memory location simultaneously, and at least one of the accesses is a write, without proper synchronization. Data races are low-level issues.

Race condition is more about logical errors caused by incorrect sequencing or coordination of tasks

## Source

https://www.geeksforgeeks.org/cpp/volatile-qualifier-in-cpp/

https://www.amazon.com/Low-Latency-Trading-Insights-Performance/dp/B0GG5M4BN7

https://abdulahd1996.medium.com/data-races-vs-race-conditions-understanding-the-differences-in-concurrency-602042cd54b2