# Object Slicing

## Example

Imagine you have a code like this

```c++
#include <iostream>
#include <string>

class Base {
public:
    virtual std::string getName() {
        return "Base";
    }
};

class Derived : public Base {
public:
    std::string getName() override {
        return "Derived";
    }
};

int main () {
    Base base;
    Derived derived;

    base = derived;
    
    std::cout << base.getName() << std::endl;
}
```

The output is
```
Base
```

We don't expect this to happen.

To fix this, we can use pointer or using reference

Pointer approach
```c++
int main () {
    Base* base;
    Derived derived;

    base = &derived;

    std::cout << base->getName() << std::endl;
}
```

Reference approach
```c++
int main () {
    Derived derived;
    Base& base = derived;

    std::cout << base.getName() << std::endl;
}
```

Both outputs same
```
Derived
```

## Source

https://www.youtube.com/watch?v=99xTGkdxAdc