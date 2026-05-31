# Override / Final

## Override

`override` is actually an optional keyword, but still useful to prevent a human error from programmer.

### Code Example

Suppose you have a code like this

```c++
// A CPP program without override keyword, here
// programmer makes a mistake and it is not caught
#include <iostream>
using namespace std;

class Base {
public:
    // user wants to override this in
    // the derived class
    virtual void func() { cout << "I am in base" << endl; }
};

class derived : public Base {
public:
    // did a silly mistake by putting
    // an argument "int a"
    void func(int a)
    {
        cout << "I am in derived class" << endl;
    }
};

// Driver code
int main()
{
    Base b;
    derived d;
    cout << "Compiled successfully" << endl;
    return 0;
}
```

This still compiled

```
Compiled successfully
```

If you use `override`, compiler will catch error

```c++
// A CPP program that uses override keyword so
// that any difference in function signature is
// caught during compilation
#include <iostream>
using namespace std;

class Base
{
  public:
    // user wants to override this in
    // the derived class
    virtual void func()
    {
        cout << "I am in base" << endl;
    }
};

class derived : public Base
{
  public:
    // did a silly mistake by putting
    // an argument "int a"
    void func(int a) override
    {
        cout << "I am in derived class" << endl;
    }
};

int main()
{
    Base b;
    derived d;
    cout << "Compiled successfully" << endl;
    return 0;
}
```

```bash
main.cpp:23:10: error: ‘void derived::func(int)’ marked ‘override’, but does not override
   23 |     void func(int a) override
      |          ^~~~
```

## Final

Sometimes you don't want to allow derived class to override base class virtual function, we can use `final` keyword.

```c++
#include <iostream>
using namespace std;

class Base
{
public:
    virtual void myfun() final
    {
        cout << "myfun() in Base";
    }
};
class Derived : public Base
{
    void myfun()
    {
        cout << "myfun() in Derived\n";
    }
};

int main()
{
    Derived d;
    Base &b = d;
    b.myfun();
    return 0;
}
```

When you compile the code, this will happen

```bash
prog.cpp:14:10: error: virtual function ‘virtual void Derived::myfun()’
     void myfun()
          ^
prog.cpp:7:18: error: overriding final function ‘virtual void Base::myfun()’
     virtual void myfun() final 
```

You also can use `final` keyword to prevent a class being inherited.

```c++
#include <iostream>
class Base final { };

class Derived : public Base { };

int main()
{
    Derived d;
    return 0;
}
```

When you compile it

```bash
error: cannot derive from ‘final’ base ‘Base’ in derived type ‘Derived’
 class Derived : public Base
```


## Source

https://www.geeksforgeeks.org/cpp/override-keyword-c/

https://www.geeksforgeeks.org/cpp/c-final-specifier/