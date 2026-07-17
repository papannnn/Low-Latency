# Low-Level Primitives

## `atomic_flag`

`std::atomic_flag` is an atomic boolean type. Unlike all specializations of `std::atomic`, it is guaranteed to be lock-free. Unlike `std::atomic<bool>`, `std::atomic_flag` does not provide load or store operations.

### `test_and_set`

atomically sets the flag to `true` and obtains its previous value

```c++
std::atomic_flag flag = ATOMIC_FLAG_INIT;

std::cout << flag.test_and_set(); // false
std::cout << flag.test_and_set(); // true
std::cout << flag.test_and_set(); // true
```

- Fence instructions and memory barriers
## `std::atomic_thread_fence`

Suppose there are many atomics:

```c++
counter.store(...);
version.store(...);
state.store(...);
flag.store(...);
```

Maybe you want all of them relaxed for performance.
```c++
obj.a = ...;
obj.b = ...;
obj.c = ...;

std::atomic_thread_fence(std::memory_order_release);

counter.store(..., relaxed);
version.store(..., relaxed);
state.store(..., relaxed);
flag.store(..., relaxed);
```

## Source

https://en.cppreference.com/cpp/atomic/atomic_flag

https://en.cppreference.com/cpp/atomic/atomic_thread_fence