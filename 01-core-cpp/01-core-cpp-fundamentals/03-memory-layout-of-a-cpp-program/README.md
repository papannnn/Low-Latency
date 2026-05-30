# Memory Layout of a C++ Program

## Definition

In C++, memory layout look like this

```
[High Address]
Stack (Grow Downward)
  ↓ 
  ↑ 
Heap (Grow Upward)
Uninitialized Data (.bss) (Initialized to zero at runtime)
Initialized Data
Text (Read from program file)
[Low Address]
```

## Text Segment (Code Segment)

It's read only data to enhance safety, more code you write = more bigger it's size

## Data Segment

Data segment stores global and static variable. It's allocated above Text Segment.

Data Segment divided into 2 type:

### Initialized Data

```c++
#include <iostream>
using namespace std;

// Global initialized variable
int a = 10;

// Static initialized variable
static int b = 20;

int main() {
    cout << "Global initialized: " << a << endl;
    cout << "Static initialized: " << b << endl;
    return 0;
}
```

### Uninitialized Data (.bss)

```c++
#include <iostream>
using namespace std;

// Global uninitialized variable
int c;

// Static uninitialized variable
static int d;

int main() {
    cout << "Global uninitialized: " << c << endl;
    cout << "Static uninitialized: " << d << endl;
    return 0;
}
```

Only global and static variable that will be automatically set into 0 if there's no definition. Other than that will use garbage value if not assigned.

## Heap Segment

Already explained in [here](../02-stack-vs-heap/README.md)

## Stack Segment

Already explained in [here](../02-stack-vs-heap/README.md)

## Source

https://www.geeksforgeeks.org/cpp/memory-layout-of-cpp-program/