# Partial Specialization

## Usecase

Assuming you have class template like this

```c++
template <class T, class X>
class Geek {
      // General implementation for the primary template
};
```

You want to do specialization, but only partial

```c++
template < class T>
class Geek<T, int> {
      //  Specialized implementation for the primary template
}
```

If you want to do it on function template, it's also same

## Example

```c++
// C++ Program to illustrate Class Template Partial
// Specialization
#include <iostream>
using namespace std;

// Primary template
template <typename T, typename X>
class Geek {
public:
    void Print() { cout << "Primary Template" << endl; }
};

// Partial specialization for X as int
template <typename T> class
Geek<T, int> {
public:
    void Print()
    {
        cout << "Partial Specialization for int" << endl;
    }
};

// driver code
int main()
{
    Geek<bool, double> obj1;
    Geek<bool, int> obj2;

    obj1.Print();
    obj2.Print();
    return 0;
}
```

Output

```
Primary Template
Partial Specialization for int
```

## Source

https://www.geeksforgeeks.org/cpp/partial-template-specialization-in-cpp/