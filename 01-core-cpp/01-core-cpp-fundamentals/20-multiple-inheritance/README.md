# Multiple Inheritance

## Simple Class

Let's take a step back and see how simple class works.

```c++
class Simple {
public:
    double GetVal() const {
        return val;
    }
private:
    double val;
};
```

In simplified version, what compiler does is doing this to `GetVal()` function, it will create a global function that tied to `Simple` class.

```c++
inline double Simple::GetVal(const Simple* this) {
  return this->val;
}
```

Then, when you use `GetVal` method, it will do something like this
```c++
// What you see
double val = obj.GetVal();

// What exactly compiler do
double val = GetVal(&obj); // resolved like global free functions
```

The memory layout for `Simple` class will look like this

```
+----------------------+
|     double val       |
+----------------------+
        Simple
```

## Single Inheritance

Let's step up a little bit

```c++
class Base {
public:
    double b;
};

class Derived : public Base {
public:
  	double d;
};
```

The memory layout will look like this

```
+----------------------+
|     Base Subobject   |
|       double b       |
+----------------------+
|       double d       |
+----------------------+
        Derived
```

If you have virtual function

```c++
class Base {
public:
    virtual ~Base();
    virtual void Foo();
    virtual void Bar();
    double b;
};

class Derived : public Base {
public:
    void Foo() override; // override
    virtual void Baz(); // new virtual function
    double d;
  	
};
```

The memory layout will look like this

`Base` class
```
+----------------------+
|         vptr         | ----+
+----------------------+     |
|       double b       |     |
+----------------------+     |
           Base              |
                              v
                  +----------------------+
                  |      top offset      |
                  +----------------------+
                  |   typeinfo for Base  |
                  +----------------------+
                  |    ~Base::~Base()    |
                  +----------------------+
                  |     Base::Foo()      |
                  +----------------------+
                  |     Base::Bar()      |
                  +----------------------+
                           VTable
```

And `Derived` class will look like this

```
+----------------------+
|     Base Subobject   | ----+
|         vptr         |     |
|       double b       |     |
+----------------------+     |
|       double d       |     |
+----------------------+     |
         Derived             |
                             v
                  +----------------------+
                  |      top offset      |
                  +----------------------+
                  | typeinfo for Derived |
                  +----------------------+
                  |   Derived::~Derived  |
                  +----------------------+
                  |    Derived::Foo()    |
                  +----------------------+
                  |      Base::Bar()     |
                  +----------------------+
                  |    Derived::Baz()    |
                  +----------------------+
                           VTable
```

When you do
```c++
Base *base = new Derived();
base->Foo();
```

Under the hood is actually doing something like this
```c++
*(pB->vptr[2])(pB);
```

## Multiple Inheritance

Let's step up into multiple inheritance

```c++
// C++ program to show inheritance ambiguity

#include<iostream>
using namespace std;

// Base class A

class A {
    public:

    void func() {
        cout << " I am in class A" << endl;
    }
};

// Base class B

class B {
    public:

    void func() {
        cout << " I am in class B" << endl;
    }
};

// Derived class C

class C: public A, public B {


};

// Driver Code

int main() {

    // Created an object of class C

    C obj;

    // Calling function func()

    obj.func();
  
    return 0;
}
```

This will result
```bash
prog.cpp: In function ‘int main()’:
prog.cpp:43:9: error: request for member ‘func’ is ambiguous
     obj.func();
         ^
prog.cpp:21:10: note: candidates are: void B::func()
     void func() {
          ^
prog.cpp:11:10: note:                 void A::func()
     void func() {
          ^
```

To fix this, you can do

```c++
ObjectName.ClassName::FunctionName();
```

```c++
// C++ program to resolve inheritance
// ambiguity

#include<iostream>
using namespace std;

// Base class A

class A {
    public:

    void func() {
        cout << " I am in class A" << endl;
    }
};

// Base class B

class B {
    public:

    void func() {
        cout << " I am in class B" << endl;
    }
};

// Derived class C
class C: public A, public B {


};

// Driver Code

int main() {

    // Created an object of class C
    C obj;

    // Calling function func() in class A
    obj.A::func();

    // Calling function func() in class B
    obj.B::func();

    return 0;
}
```

## Source

https://nimrod.blog/posts/what-does-cpp-object-layout-look-like/

https://sofiabelen.github.io/projects/visualizing-the-cpp-object-memory-layout-part-1-single-inheritance/

https://www.geeksforgeeks.org/cpp/inheritance-ambiguity-in-cpp/