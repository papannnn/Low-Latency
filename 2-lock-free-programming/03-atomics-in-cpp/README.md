# Atomics in C++

The concept behind false sharing is especially important.

## Topics

- `std::atomic<T>`:
  - `store()`, `load()`, `exchange()`
  - `compare_exchange_strong` vs `compare_exchange_weak`
- Custom atomic types using `std::atomic_flag`, `atomic<uint64_t>`, etc.
- Alignment and padding, especially false sharing

## Note

Try finishing all this within one weekend. Do not ignore the experiment part.
