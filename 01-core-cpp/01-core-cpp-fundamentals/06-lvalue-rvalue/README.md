# Lvalue / Rvalue

## Code Example

lvalue is something that has a location to store that.

rvalue is something that doesn't have location to store that.

```c++
#include <iostream>

int main () {
    int i = 10;
}
```

`i` variable is lvalue, `10` is rvalue.

You can do this on lvalue
```c++
int i = 10
```

But can't do this on rvalue
```c++
10 = 20; // error
10 = i // error
```

`getValue` return rvalue
```c++
int getValue() {
    return 10;
}
```

Because you can't do this
```c++
getValue() = 10;
```

You can make a function returning lvalue
```c++
int value = 10;

int& get() {
    return value;
}
```

Now you can do
```c++
get() = 10;
```

Currently this function accept both lvalue and rvalue
```c++
void setValue(int value) {
    //
}

int main () {
    int a = 10;
    setValue(a);
    setValue(20);
}
```

But, if you change it into
```c++
void setValue(int& value) {
    //
}
```

The code will error
```c++
setValue(a); // Safe
setValue(20); // Error, since it expect lvalue
```

Also, if you change it into
```c++
void setValue(const int& value) {
    //
}
```

This code will works again
```c++
setValue(a); // Safe
setValue(20); // Safe
```

Adding 2 data will make another rvalue

```c++
void printValue(int& value) {
    //
}

int main () {
    int a = 10;
    int b = 20;

    printValue(a); // safe
    printValue(b) // safe
    printValue(a + b); // Error because a + b is rvalue
}
```

If we change the function into
```c++
void printValue(int&& value) {
    //
}
```

Now the error is changed
```c++
int main () {
    int a = 10;
    int b = 20;

    printValue(a); // Error because function expect rvalue
    printValue(b) // Error because function expect rvalue
    printValue(a + b); // Safe
}
```

## Source

https://www.youtube.com/watch?v=fbYknr-HPYE