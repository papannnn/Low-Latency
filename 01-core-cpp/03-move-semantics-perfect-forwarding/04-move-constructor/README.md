# Move Constructor

## Syntax

```c++
ClassName(ClassName&& other);
```

## Why move constructor are used

To transfer resource from an object to another object without copying, since copying is slower than moving.

## Noexcept move constructor

If move constructor is not marked as noexcept, some stl will fallback using copy to avoid potential exceptions.

### Example 1: Without noexcept move constructor

```c++
// C++ program to illustrate what happens when we
// don't use the noexcept move constructor
#include <bits/stdc++.h>
using namespace std;

class A {
public:
    A() {}
  
    // Move constructor not marked as noexcept
    A(A&& other) {
        cout << "Move constructor" << endl;
    }

    // Copy constructor
    A(const A& other) {
        cout << "Copy constructor" << endl;
    }
};

int main() {
    vector<A> v;

  	// Reserve space for at least two elements to
	// avoid immediate resizing
    v.reserve(2);

    // Uses the move constructor for the temporary objects
    v.push_back(A());
    v.push_back(A());

  	cout << "Resize happens" << endl;
    
    // Move constructor may be called again if resizing occurs
    v.push_back(A());
    
    return 0;
}
```
```
Move constructor
Move constructor
Resize happens
Move constructor
Copy constructor
Copy constructor
```

### Example 2: With noexcept Move Constructor

```c++
// C++ Program to illustrate the use of
// noexcept move constructor
#include <vector>
#include <iostream>
using namespace std;

class Test {
public:
    Test(){}
      
    // Move constructor marked as noexcept
    Test(Test&& other) noexcept {
        cout << "Move constructor  " << endl;
    }

    // Copy constructor
    Test(const Test& other) {
        cout << "Copy constructor  " << endl;
    }
};

int main() {
    vector<Test> vec;

    vec.reserve(2);  // Reserve space for at least two elements

  Test a;

    vec.push_back(Test());
    vec.push_back(Test());  // Uses the move constructor

    cout << "Resize happens" << endl;
    vec.push_back(Test());
    
    return 0;
}
```
```
Move constructor  
Move constructor  
Resize happens
Move constructor  
Move constructor  
Move constructor  
```

- Stealing resources
- `noexcept` move constructors
- Moved-from object state
- Rule of 5

## Source

https://www.geeksforgeeks.org/cpp/move-constructors-in-c-with-examples/