# Template Specialization

## Usecase

We need template specialization when we need to have some specific code in 1 type.

```c++
template <typename T>
class ClassName {
    // generic definition
};
```

```c++
template <>
class ClassName<int> {
    // specialized definition for int
};
```

Full example code

```c++
#include <iostream>
using namespace std;

// Generic Template
template <typename T>
class Printer {
public:
    void print(T data) {
        cout << "Generic Printing: " << data << endl;
    }
};

// Specialization for char
template <>
class Printer<char> {
public:
    void print(char data) {
        cout << "Character Printing: " << data << endl;
    }
};

int main() {
    Printer<int> p1;
    p1.print(100);     // Generic Printing: 100

    Printer<string> p2;
    p2.print("Hello"); // Generic Printing: Hello

    Printer<char> p3;
    p3.print('A');     // Character Printing: A

    return 0;
}
```

Outside class template specialization, we can do it on function template

```c++
#include <iostream>
using namespace std;

// Generic function template
template <typename T>
void display(T value) {
    cout << "Generic display: " << value << endl;
}

// Specialized version for char
template <>
void display<char>(char value) {
    cout << "Specialized display for char: '" << value << "'" << endl;
}

int main() {
    display(42);         // Calls the generic version
    display(3.14);       // Calls the generic version
    display('A');        // Calls the specialized version
    display("Hello");    // Calls the generic version (as const char*)

    return 0;
}
```

## Source

https://www.geeksforgeeks.org/cpp/template-specialization-c/
