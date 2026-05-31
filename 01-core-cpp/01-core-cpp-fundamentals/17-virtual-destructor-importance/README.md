# Virtual Destructor Importance

## Virtual Destructor

Previously on [virtual function](../15-virtual-functions/README.md)

We did this code

```c++
#include <iostream>

class Base {
public:
    Base() {
        std::cout << "Base constructor" << std::endl;
    }

    ~Base() {
        std::cout << "Base destructor" << std::endl;
    }

    virtual void MemberFunc() {
        std::cout << "Base::MemberFunc()" << std::endl;
    }
};

class Derived : public Base {
public:
    Base() {
        std::cout << "Derived constructor" << std::endl;
    }

    ~Base() {
        std::cout << "Derived destructor" << std::endl;
    }

    void MemberFunc() override {
        std::cout << "Derived::MemberFunc()" << std::endl;
    }
};

int main () {
    Base *instance = new Derived;
    delete instance;
}
```

But the output is not expected

```
Base constructor
Derived constructor
Base destructor
```

This is dangerous, because Destructor is not called, what if Derived class allocate some dynamic memory? It will cause memory leak.

To fix it, you need to add `virtual` on Base class

```c++
```c++
#include <iostream>

class Base {
public:
    Base() {
        std::cout << "Base constructor" << std::endl;
    }

    virtual ~Base() {
        std::cout << "Base destructor" << std::endl;
    }

    virtual void MemberFunc() {
        std::cout << "Base::MemberFunc()" << std::endl;
    }
};

class Derived : public Base {
public:
    Base() {
        std::cout << "Derived constructor" << std::endl;
    }

    ~Base() {
        std::cout << "Derived destructor" << std::endl;
    }

    void MemberFunc() override {
        std::cout << "Derived::MemberFunc()" << std::endl;
    }
};

int main () {
    Base *instance = new Derived;
    delete instance;
}
```
Now the output will be correct
```
Base constructor
Derived constructor
Derived destructor
Base destructor
```

## Source

https://youtu.be/XvzgC5WoBCE?si=YbA-_YPgEPW-di2L
