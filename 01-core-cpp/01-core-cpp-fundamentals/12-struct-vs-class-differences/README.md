# Struct vs Class Differences

## Differences

In C++, struct works the same way like class, except the default member of class are private, and struct are public.

### Class example code

```c++
// C++ Program to demonstrate that
// Members of a class are private
// by default
#include <iostream>

using namespace std;

class Test {
    // x is private
    int x;
};

int main() {
    Test t;
  
    // compiler error because x
    // is private
    t.x = 20;

    return t.x;
}
```
```bash
./cf03c8d1-d4a3-43ea-a058-fe5b5303167b.cpp: In function 'int main()':
./cf03c8d1-d4a3-43ea-a058-fe5b5303167b.cpp:10:9: error: 'int Test::x' is private
     int x;
         ^
./cf03c8d1-d4a3-43ea-a058-fe5b5303167b.cpp:18:7: error: within this context
     t.x = 20;
       ^
./cf03c8d1-d4a3-43ea-a058-fe5b5303167b.cpp:10:9: error: 'int Test::x' is private
     int x;
         ^
./cf03c8d1-d4a3-43ea-a058-fe5b5303167b.cpp:20:14: error: within this context
     return t.x;
              ^
```

### Example code struct

```c++
// C++ Program to demonstrate that
// members of a structure are public
// by default
#include <iostream>

using namespace std;

struct Test {
    // x is public
    int x;
};

int main()
{
    Test t;
    t.x = 20;

    // works fine because x is public
    cout << t.x;
}
```
```bash
20
```

## Intent

Class is normally used for OOP, while Structure is to group of different datatypes.

## Source

https://www.geeksforgeeks.org/cpp/structure-vs-class-in-cpp/
