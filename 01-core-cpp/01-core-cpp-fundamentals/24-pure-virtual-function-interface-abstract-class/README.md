# Pure Virtual Function, Interface, Abstract Class

## Pure Virtual Function

Pure virtual function is a function that has no implementation in base class. Declared using `= 0`.

A class that at least having 1 pure virtual function is an abstract class. Abstract class can't be instantiated.

```c++
#include <iostream>
using namespace std;

class Shape {
public:
    virtual void draw() = 0; // Pure virtual function
};

class Circle : public Shape {
public:
    void draw() override {
        cout << "Drawing Circle\n";
    }
};

int main() {
    
    // Error in the below line : Cannot create 
    // object of abstract class
    // Shape s;      
    
    // Pointer to abstract class
    Shape* s = new Circle(); 
    
    // Output: Drawing Circle
    s->draw();    
    
    delete s;
}
```

## Source

https://www.geeksforgeeks.org/cpp/pure-virtual-functions-and-abstract-classes/
