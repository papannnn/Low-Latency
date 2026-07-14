# Atomics in C++

The concept behind false sharing is especially important.

## Atomic

The `<atomic>` header, introduced in C++11, provides atomic types and operations for safely accessing shared data in multithreaded programs. It helps prevent data races by ensuring that operations on shared variables are performed atomically.

### Common Atomic Operation

| Function    | Description                                                |
|-------------|------------------------------------------------------------|
|    load()   |       Reads the current value of the atomic variable.      |
|   store()   |        Stores a new value into the atomic variable.        |
|  exchange() | Replaces the current value and returns the previous value. |
| fetch_add() |     Atomically adds a value and returns the old value.     |
| fetch_sub() |   Atomically subtracts a value and returns the old value.  |
| fetch_and() |          Performs an atomic bitwise AND operation.         |
|  fetch_or() |          Performs an atomic bitwise OR operation.          |
| fetch_xor() |          Performs an atomic bitwise XOR operation.         |

### Compare_Exchange

There's 2 type of Compare_Exchange, `compare_and_exchange_weak` & `compare_and_exchange_strong`.

`compare_and_exchange_strong` will wait for it's turn to do exchange when another thread already own exclusive lock.

`compare_and_exchange_weak` will auto fail when another thread already own the exclusive lock.

## Source

https://www.geeksforgeeks.org/cpp/cpp-11-atomic-header/

https://www.youtube.com/watch?v=EfmxxeXSzAo