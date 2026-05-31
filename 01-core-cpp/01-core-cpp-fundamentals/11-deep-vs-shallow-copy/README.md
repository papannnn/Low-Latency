# Deep vs Shallow Copy

## Shallow Copy

In Shallow Copy, all variable value are copied, but for dynamically allocated memory address, it only copy the memory address.

```c++
#include <iostream>
using namespace std;

int main() {
    int a[3] = {10, 20, 30};
    int* b = a;   // shallow copy of address

    b[0] = 99;
    cout << a[0] << endl;
    cout << b[0] << endl;
}
```
```bash
99
99
```

Another example
```c++
#include <iostream>
using namespace std;

class Sample {
public:
    int *x;

    Sample(int val) {
        x = new int;
        *x = val;
    }
};

int main() {
    Sample obj1(10);     // Original object
    Sample obj2 = obj1; // Shallow copy

    *obj2.x = 50;       // Modify copied object

    cout << "obj1.x = " << *obj1.x << endl;
    cout << "obj2.x = " << *obj2.x << endl;

    return 0;
}
```
```
obj1.x = 50
obj2.x = 50
```

## Deep Copy

On deep copy, new object is created by copying all variable value and allocating another memory for dynamically allocated value.

```c++
#include <iostream>
using namespace std;

int main() {
    int a[3] = {10, 20, 30};   
    int b[3];               

    for (int i = 0; i < 3; i++)
        b[i] = a[i];
        
    b[0] = 99;
    cout << a[0] << endl;
    cout << b[0] << endl;
    return 0;
}
```

```bash
10
99
```

Another example
```c++
#include <iostream>
using namespace std;

class Sample {
public:
    int *x;
    Sample(int val) {
        x = new int;
        *x = val;
    }
    // Deep Copy Constructor
    Sample(const Sample &obj) {
        x = new int;
        *x = *(obj.x);
    }
};

int main() {
    Sample obj1(10);      // Original object
    Sample obj2 = obj1;  // Deep copy

    *obj2.x = 50;        // Modify copied object

    cout << "obj1.x = " << *obj1.x << endl;
    cout << "obj2.x = " << *obj2.x << endl;

    return 0;
}
```
```
obj1.x = 10
obj2.x = 50
```

## Source

https://www.geeksforgeeks.org/cpp/shallow-copy-and-deep-copy-in-c/