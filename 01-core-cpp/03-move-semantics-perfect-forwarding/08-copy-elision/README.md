# Copy Elision

## What is Copy Elision

Is a compiler way to cut the middleman.

When you pass an object by value or return one from a function, the usual process involves making a temporary copy of that object.

However, with copy elision, the compiler skips creating these temporary copies and instead constructs the object directly where it’s needed.

## Example without copy elision

```c++
#include <iostream>

class MyClass {
public:
    MyClass() { std::cout << "Default constructor\n"; }
    MyClass(const MyClass&) { std::cout << "Copy constructor\n"; }
    ~MyClass() { std::cout << "Destructor\n"; }
};

MyClass createObject() {
    MyClass obj;
    return obj;
}

int main() {
    MyClass myObject = createObject();
    return 0;
}
```

- Inside the createObject() function, an object obj is created.
- When the object obj is returned, it’s copied to a temporary object.
- That temporary object is then copied again into myObject in main().

Output Without Copy Elision

```
Default constructor
Copy constructor
Copy constructor
Destructor
Destructor
Destructor
```

## How Copy Elision Fixes This

With copy elision, the unnecessary copies disappear. 

The object is constructed directly where it’s needed, avoiding all the extra copying. 

In C++17, this optimization is automatically applied in many cases.

Here’s what happens with Return Value Optimization (RVO):

```c++
MyClass createObject() {
    MyClass obj;
    return obj;  // RVO applies here
}
```

Output

```c++
Default constructor
Destructor
```

## Mandatory Copy Elision in C++17

In C++17, copy elision is guaranteed in certain situations. 

For instance, when you return a temporary object from a function, the compiler is required to optimize away any unnecessary copies.

```c++
MyClass createObject() {
    return MyClass();  // C++17 mandates copy elision
}
```

## Visualization

Without Copy Elision (Multiple Copies)

```
createObject()             main()
  ┌───────────┐          ┌────────────┐
  │   obj     │          │  myObject  │
  └───────────┘  copy →  └────────────┘
      │                       │
      ↓                       ↓
   Destructor             Destructor
```

With Copy Elision (Direct Construction)

```
createObject()             main()
  ┌───────────┐
  │ myObject  │ (constructed directly)
  └───────────┘
      ↓
   Destructor
```

## Source

http://medium.com/@tusharmalhotra_81114/copy-elision-in-c-a-simple-way-to-optimize-your-code-be33f492920e

https://www.geeksforgeeks.org/cpp/copy-elision-in-cpp/