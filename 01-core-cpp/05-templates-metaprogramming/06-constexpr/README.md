# `constexpr`

## Reason

The reason of using `constexpr` are

- Enables computations to be evaluated at compile time
- Helps reduce runtime overhead and improve performance
- Allows functions and variables to be used in constant expressions

## Example

```c++
#include <iostream>

constexpr int product(int x, int y) { return (x * y); }

int main()
{
    constexpr int x = product(10, 20);
    std::cout << x;
    return 0;
}
```

Because `constexpr` is running at compile time, that means you can use it for array sizes

```c++
constexpr int product(int x, int y) { return (x * y); }

int main()
{
    int arr[product(2, 3)] = {1, 2, 3, 4, 5, 6};
    std::cout << arr[5];
    return 0;
}
```

### Requirement

- The function body can contain only a single return statement.
- It can use only constant expressions and constant global variables.
- It can call only other constexpr functions.
- The return type cannot be void.
- Loops, local variables, and many operations were not allowed


## `constexpr` constructor

This allow an object created at compile time.

```c++
#include <iostream> 

// A class with constexpr 
// constructor and function 
class Rectangle 
{ 
    int _h, _w; 
public: 
    // A constexpr constructor 
    constexpr Rectangle(int h, int w) : _h(h), _w(w) {} 
    
    constexpr int getArea() const { return _h * _w; } 
}; 

// driver program to test function 
int main() 
{ 
    // Below object is initialized at compile time 
    constexpr Rectangle obj(10, 20); 
    std::cout << obj.getArea(); 
    return 0; 
}
```

### Requirement

- The class must not have virtual base classes.
- Constructor arguments must be usable in constant expressions.
- The constructor cannot be a function-try-block.
- A constexpr constructor is implicitly inline.


## Source

https://www.geeksforgeeks.org/cpp/understanding-constexper-specifier-in-cpp/