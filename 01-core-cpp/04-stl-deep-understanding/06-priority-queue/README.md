# `std::priority_queue`

## How element are stored

Under the hood, `priority_queue` is using heap data structure, by default it's using Max Heap.

## Time complexity

| Operation              | Complexity   |
| ---------------------- | ------------ |
| `top()`                | **O(1)**     |
| `push()`               | **O(log n)** |
| `emplace()`            | **O(log n)** |
| `pop()`                | **O(log n)** |
| `empty()`              | **O(1)**     |
| `size()`               | **O(1)**     |
| Constructor from range | **O(n)**     |
| `swap()`               | **O(1)**     |

## Source

https://www.geeksforgeeks.org/cpp/priority-queue-in-cpp-stl/