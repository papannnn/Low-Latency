# Inline Keyword, Static Keyword, Namespace

## Inline

Inline function is a code that got expanded in compile time to avoid function call overhead.

Assuming your code looks like this

```c++
inline void displayNum(int num) {
    std::cout << num << std::endl;
}

int main () {
    displayNum(5);
    displayNum(15);
    displayNum(20);
}
```

When it's compiled, it actually looks like this

```c++
int main () {
    std::cout << 5 << std::endl;
    std::cout << 15 << std::endl;
    std::cout << 20 << std::endl;
}
```

Using inline making the code more bigger, so use it wisely.

## Static

`static` keyword has so many usage

### Static in function

When a variable inside a function is declared as static, it is allocated once for the entire lifetime of the program rather than on each function call.

```c++
#include <bits/stdc++.h>
using namespace std;

void f() {
    // Static variable
    static int count = 0;
    count++;
  	cout << count << " ";
}

int main() {
  	// Calling function f() 5 times
    for (int i = 0; i < 5; i++)
        f();
  
    return 0;
}
```

```
1 2 3 4 5 
```

### Static data member in class

The data will be owned by the class, not per object.

```c++
#include <iostream>
using namespace std;

class GfG {
public:
    static int i;
    GfG(){
        // Do nothing
    };
};

// Static member inintialization
int GfG::i = 1;

int main() {
  
    // Prints value of i
    cout << GfG::i;
}
```

### Static Member Functions in a Class

Static member function belong to class rather than belong to object. And can only be accessed by scope resolution `::`

```c++
#include <iostream>
using namespace std;

class GfG {
public:
    // Static member function
    static void printMsg() { cout << "Welcome to GfG!"; }
};

int main() {
    // Invoking a static member function
    GfG::printMsg();
}
```

### Global Static Variable

This can be used if you don't want to share a variable / function outside the file.

```c++
#include <iostream>
using namespace std;

// Global static variable
static int count = 0;

void increment() {
    count++;
    cout << count << " ";
}

int main() {
    increment();
    increment();
    return 0;
}
```

## Namespace

Namespace is used to organize identifier like variable, class, function, etc.

It help to prevent name conflict.

```c++
#include <iostream>  
namespace Room1 {
    // Function greet inside namespace Room1
    void greet() {
        std::cout << "Hello from Room 1!" << std::endl;
    }
}

namespace Room2 {
    // Function greet inside namespace Room2
    void greet() {
        std::cout << "Hello from Room 2!" << std::endl;
    }
}

int main() {
    // Use the scope resolution operator (::) to access greet() function inside namespace Room1
    Room1::greet();  
    Room2::greet(); 
    
    return 0;  
}
```

### Namespace using directive

To avoid writing the `namespace::something`, we can use 

`using namespace namespace_name`

```c++
#include <iostream>

namespace first_space {
    void func() {
        std::cout << "Inside first_space"
            << std::endl;
    }
}

// Using first_space
using namespace first_space;

int main() {
    
    // Call the method of first_space
    func();  
    return 0;
}
```

### Using declaration

We can also use `using` for specific thing in namespace instead of taking whole namespace

```c++
#include <iostream>

namespace first_space {
    void func() {
        std::cout << "Inside first_space" << std::endl;
    }
}

// Using first_space
using first_space::func;

int main() {
    
    // Call the method of first_space
    func();  
    return 0;
}
```

### Nested namespace

```c++
#include <iostream>
using namespace std;

// Outer namespace
namespace outer {
    void fun(){
        cout << "Inside outer namespace" << endl;
    }
    
    // Inner namespace
    namespace inner {
        void func() {
            cout << "Inside inner namespace";
        }
    }
}

int main() {
    
    // Accessing member of inner 
    // namespace
    outer::inner::func();  
    return 0;
}
```

### Global namespace

By default, something that doesn't have namespace actually having a global namespace

```c++
#include <bits/stdc++.h>
using namespace std;

int n = 3;

int main() {
    int n = 7;
    
    // Accessing global namespace
    cout << ::n << endl;
    cout << n;

    return 0;
}
```

```
3
7
```

### Extending namespace

We can also add more to existing namespace

```c++
#include <bits/stdc++.h>
using namespace std;

namespace nmsp{
    void func(){
        cout << "You can extend me" << endl;
    }
}

// Extending the same namespace
namespace nmsp{
    void func2(){
        cout << "Adding new feature";
    }
}

int main() {
    nmsp::func();
    nmsp::func2();

    return 0;
}
```

## Source

https://www.geeksforgeeks.org/cpp/inline-functions-cpp/

https://www.geeksforgeeks.org/cpp/static-keyword-cpp/

https://www.geeksforgeeks.org/cpp/namespace-in-c/