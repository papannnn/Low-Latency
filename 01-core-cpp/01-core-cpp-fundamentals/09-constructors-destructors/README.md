# Constructors / Destructors

## Constructor

Constructor is a special method that got called when an object of a class is created.

```c++
#include <iostream>
using namespace std;

class A {
public:
  
    // Constructor without any parameters
    A() {
        cout << "Constructor called" << endl;
    }
};

int main() {
    A obj1;
    return 0;
}
```

```bash
Constructor called
```

## Type of Constructor

### Default Constructor

A default constructor will be created automatically by a compiler when no constructor defined.

This also not generated when user manually creates a destructor

```c++
#include <iostream>
using namespace std;

// Class with no explicity defined constructors
class A {
public:
};

int main() {
  
  	// Creating object
    A a;
    return 0;
}
```

### Parameterized Constructor

Parameterized Constructor let you to pass argument to help you initialize object member.

```c++
#include <iostream>
using namespace std;

class A {
public:
    int val;
  
  	// Parameterized Constructor
    A(int x) {
        val = x;
    }
};

int main() {
  
  	// Creating object with a parameter
    A a(10);
    cout << a.val;
    return 0;
}
```
```bash
10
```

### Copy Constructor

Copy Constructor is a constructor that the parameter is it's own class object reference.

```c++
#include <iostream>
using namespace std;

class A {
public:
    int val;
    
    // Parameterized constructor
    A(int x) {
        val = x;
    }
    
    // Copy constructor
    A(A& a) {
        val = a.val;
    }
};

int main() {
    A a1(20);
    
    // Creating another object from a1
    A a2(a1);
    
  	cout << a2.val;
    return 0;
}
```
```bash
20
```

### Move Constructor

Move Constructor steal the object resources and move it into the current object that being created. It avoid unnecessary copy.

```c++
#include <iostream>
#include <vector>
using namespace std; 

class MyClass {
private:
    int b;

public:
    // Constructor
    MyClass(int &&a) : b(move(a)) {
        cout << "Move constructor called!" << endl;
    }

    void display() {
            cout << b <<endl;
    }
};

int main() {
    int a = 4;
    MyClass obj1(move(a));  // Move constructor is called

    obj1.display();
    return 0;
}
```
```bash
Move constructor called!
4
```

## Destructor

Destructor is also another special member function, it's automatically called when object is out of scope or destroyed. It's uses is to free resources.

```c++
#include <iostream>
using namespace std;

class Test {
public:

    // User-Defined Constructor
    Test() {
        cout << "Constructor Called"
             << endl;
    }

    // User-Defined Destructor
    ~Test() {
        cout << "Destructor Called"
             << endl;
    }
};
int main() {
    Test t;

    return 0;
}
```
```bash
Constructor Called // When Test t;
Destructor Called // When main() function finished
```

### When do we need a user defined destructor?

If a class has a dynamic allocated memory, we must use destructor

```c++
#include <iostream>
using namespace std;

class MyClass {
private:

    // Pointer to dynamically 
    // allocated memory
    int* data;

public:
    MyClass(int value) {
        data = new int;
        *data = value;
        cout << *data << endl;
    }

    // User-defined destructor: Free 
    // the dynamically allocated memory
    ~MyClass() {
        
        // Deallocate the dynamically 
        // allocated memory
        delete data;  
        cout << "Destructor: Memory deallocated";
    }
};

int main() {
    MyClass obj1(10);
    return 0;
}
```
```bash
10
Destructor: Memory deallocated
```

### How to call destructor explicitly?

```c++
object_name.~class_name()
```

## Order of Constructor / Destructor Call C++

### Single Inheritance
```c++
// C++ program to show the order of constructor call
// in single inheritance
 
#include <iostream>
using namespace std;

// base class
class Parent
{
    public:
    
    // base class constructor
    Parent()
    {
        cout << "Inside base class" << endl;
    }
};

// sub class
class Child : public Parent
{
    public:
    
    //sub class constructor
    Child()
    {
        cout << "Inside sub class" << endl;
    }
};

// main function
int main() {
     
    // creating object of sub class
    Child obj;
    
    return 0;
}
```
```bash
Inside base class
Inside sub class
```

### Multiple Inheritance

```c++
// C++ program to show the order of constructor calls 
// in Multiple Inheritance

#include <iostream>
using namespace std;

// first base class
class Parent1
{   
   
    public:
    
    // first base class's Constructor    
    Parent1()
    {
        cout << "Inside first base class" << endl;
    }
};

// second base class
class Parent2
{
    public:
    
    // second base class's Constructor
    Parent2()
    {
        cout << "Inside second base class" << endl;
    }
};

// child class inherits Parent1 and Parent2
class Child : public Parent1, public Parent2
{
    public:
    
    // child class's Constructor
    Child()
    {
        cout << "Inside child class" << endl;
    }
};

// main function
int main() {
    
    // creating object of class Child
    Child obj1;
    return 0;
}
```
```bash
Inside first base class
Inside second base class
Inside child class
```

## Demo Code

Assuming there's a code like this

```c++
#include <iostream>

class A {
public:
    A() {
        std::cout << "A ctor\n";
    }

    ~A() {
        std::cout << "A dtor\n";
    }
};

class B {
public:
    B() {
        std::cout << "B ctor\n";
    }

    ~B() {
        std::cout << "B dtor\n";
    }
};

class C : public A {
    B b;

public:
    C() {
        std::cout << "C ctor\n";
    }

    ~C() {
        std::cout << "C dtor\n";
    }
};

int main() {
    C c;
}
```

The output will be

```bash
// Constructor
A ctor
B ctor
C ctor

// Destructor
C dtor
B dtor
A dtor
```

How to remember it
```bash
Construction: A -> B -> C
Destruction:  C -> B -> A
```

## Source

https://www.geeksforgeeks.org/cpp/constructors-c/

https://www.geeksforgeeks.org/cpp/destructors-c/

https://www.geeksforgeeks.org/cpp/order-constructor-destructor-call-c/