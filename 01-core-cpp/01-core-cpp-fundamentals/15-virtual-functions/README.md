# Virtual Functions

## Dinamic Dispatch

Let's see this code

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

    void MemberFunc() {
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

    void MemberFunc() {
        std::cout << "Derived::MemberFunc()" << std::endl;
    }
};
```

So basically it's something like this

```
Base
  ↑
Derived
```

If we run this code

```c++
int main () {
    Base b;
    b.MemberFunc();
}
```

Output will be
```
Base constructor
Base::MemberFunc()
Base destructor
```

If we do this

```c++
int main () {
    Derived d;
    d.MemberFunc();
}
```

Output will be
```
Base constructor
Derived constructor
Derived::MemberFunc()
Derived destructor
Base destructor
```

We can also do it implicitly

```c++
int main () {
    Base* b = new Derived;
    b->MemberFunc();
}
```
Base* can point to Derived because Derived is a type of Base also

But when we run it

```
Base constructor
Derived constructor
Base::MemberFunc()
Base destructor
```

This is not expected.

We can fix this by adding virtual and override keyword

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
```

If we run it

```
Base constructor
Derived constructor
Derived::MemberFunc()
Base destructor
```

Still not working well, our destructor is not working.

Anw, if you didn't bring `override` to the code, it's actually fine, but it's better to add it.

## Source

https://www.youtube.com/watch?v=BuWehV6sD1c

https://www.geeksforgeeks.org/cpp/virtual-functions-and-runtime-polymorphism-in-cpp/