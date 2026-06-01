# Diamond Problem

## What is diamond problem

Diamond Problem happens when a Derived class inherits from two class that actually inherit from common Base class.

```
          Base
            ^
            |
    +-------+-------+
    |               |
    |               |
   Left          Right
    ^               ^
    |               |
    +-------+-------+
            |
            ^
         Derived
```

### Example code

```c++
#include <iostream>
using namespace std;

// Base class
class Base {
public:
    void fun() { cout << "Base" << endl; }
};

// Parent class 1
class Parent1 : public Base {};

// Parent class 2
class Parent2 : public Base {};

// Child class inheriting from both parents
class Child : public Parent1, public Parent2 {};

int main() {
    Child obj;
    obj.fun();   // Ambiguity error
    return 0;
}
```

Output

```bash
main.cpp:30:9: error: request for member ‘fun’ is ambiguous
   30 |     obj.fun(); // Ambiguity error
      |         ^~~
main.cpp:8:10: note: candidates are: ‘void Base::fun()’
    8 |     void fun() { cout << "Base" << endl; }
      |          ^~~
main.cpp:20:10: note:                 ‘void Base::fun()’
```

## Source

https://www.youtube.com/watch?v=7Zpuz4T4SGw

https://www.geeksforgeeks.org/cpp/diamond-problem-in-cpp/