# `std::forward`

## Universal reference

Universal reference is a reference that can bind to rvalues and lvalues.

```c++
#include <iostream>
#include <string>

template<typename T>
void func(T&& param) {
    std::cout << "Received: ";
    if constexpr (std::is_lvalue_reference_v<T>) {
        std::cout << "lvalue\n";
    } else {
        std::cout << "rvalue\n";
    }
}

int main() {
    int x = 10;
    func(x);                    // lvalue: T deduces to int&, param is int&
    func(10);                   // rvalue: T deduces to int, param is int&&
    std::string s = "hello";
    func(s);                    // lvalue: T deduces to std::string&, param is std::string&
    func(std::string("world")); // rvalue: T deduces to std::string, param is std::string&&
}
```

## Forward

If you want to preserve the value category, use `std::forward<T>(arg)`

```c++
// C++ program to illustrate the use of
// std::forward() function
#include <iostream>
#include <utility>
using namespace std;

// Function that takes lvalue reference
void UtiltyFun(int& i) {
    cout << "Process lvalue: " << i << endl;
}

// Overload of above function but it takes rvalue
// reference
void UtiltyFun(int&& i) {
    cout << "Process rvlaue: " << i << endl;
}

// Template function for forwarding arguments
// to utlityFun() 
template <typename T>
void Fun(T&& arg) {
    UtiltyFun(forward<T>(arg));
}

int main() {
    int x = 10;
  	
  	// Passing lvalue
    Fun(x);
  
  	// Passing rvalue
    Fun(move(x)); 
}
```

## Template parameter deduction

```
T& && → T& (lvalue reference)
T&& && → T&& (rvalue reference)
T& & → T&
T&& & → T&
```

## Source

https://gist.github.com/MangaD/2b284f4cd15c4869881b0a1723b3a3fd

https://www.geeksforgeeks.org/cpp/std-forward-in-cpp/
