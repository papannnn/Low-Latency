# Concepts (C++20)

## Definition

Concepts is a compile time 'predicate' (test) on our type to make sure that we can use our templated function.

```c++
#include <iostream>
#include <concepts>

template <typename T>
requires std::integral<T>
void Print(T val) {
    std::cout << "The value is: " << val << std::endl;
}

template <typename T>
requires std::integral<T>
bool isEqual(T a, T b) {
    return a == b;
}

template <typename T>
requires std::floating_point<T>
bool isEqual(T a, T b) {
    return std::fabs(a - b) < 0.0001;
}

int main () {
    Print(7);
    // Print("string"); // Compile error

    std::cout << std::boolalpha << isEqual(1, 1) << std::endl;
    std::cout << std::boolalpha << isEqual(1.0, 1.00000000000000000009) << std::endl;
}
```

If you don't want to use `requires`, you can also do this.

```c++
template <std::floating_point T>
bool isEqual(T a, T b) {
    //
}
```

Or also you can do like this

```c++
bool isEqual(std::floating_point auto a, std::floating_point auto b) {
    //
}
```

## Source

https://www.youtube.com/watch?v=VTweta35r_A