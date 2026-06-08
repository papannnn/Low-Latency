# Stack Allocation Optimizations

## Prefer stack allocation when ownership is local

Since allocating on the heap is slower since it may call system call. The performance can degrade. Use stack if possible.

## Object lifetime and scope

Object lifetime on stack is when the function is finished

But on heap when programmer init `delete`

## Small buffer optimization

Small buffer optimization is a way to avoid allocating into heap when the data is small.

`std::string`, `std::any`, `std::function` do this.

## Stack usage limits

While stack is technically faster than heap, but stack size is smaller than heap.

## Source

https://www.geeksforgeeks.org/dsa/stack-vs-heap-memory-allocation/

https://www.youtube.com/watch?v=s7InyYdFfu4