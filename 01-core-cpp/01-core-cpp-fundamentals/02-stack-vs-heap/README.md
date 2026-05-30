# Stack vs Heap

## Definition

In C++, you can put data inside Stack or Heap. Stack allocation happened in the function call stack. For heap, programmer will handle it by themself since there's no garbage collector in C++.

## Stack

When you create a local variable inside a function, you already allocate data inside stack.

And when you finished using the function, all of the data inside that function will be destroyed automatically.

```c++
int main() {
    
  // All these variables get memory
  // allocated on stack
  int a;
  int b[10];
  int n = 20;
  int c[n];
}
```

## Heap

In order to create data inside heap, you need to use `malloc` / `new`.

Heap can't be destroyed automatically, you need to use `free` / `delete`

In order to access heap allocated variable, you need to use pointer.

```c++
int main()
{
   // This memory for 10 integers
   // is allocated on heap.
   int *ptr  = new int[10];
}
```

Because it's using pointer, that means you need to access pointer variable first, then from that pointer you need to jump into the real address of the heap variable. Making that access slower than using stack variable.

## Source

https://www.geeksforgeeks.org/dsa/stack-vs-heap-memory-allocation/