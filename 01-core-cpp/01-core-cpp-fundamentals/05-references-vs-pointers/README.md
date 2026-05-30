# References vs Pointers

## Pointer

Pointer is a variable that hold memory address of another variable

## Reference

Reference is an alias to an already existing variable. No need deferencing like pointer.

## Code Example

```c++
int i = 3; 

// A pointer to variable i or "stores the address of i"
int *ptr = &i; 

// A reference (or alias) for i.
int &ref = i; 
```

## Differences

Pointer can do this
```c++
int a = 10;
int *p = &a;
// OR 
int *p;
p = &a;
```

While reference can't
```c++
int a = 10;
int &p = a; // It is correct
// but
int &p;
p = a; // It is incorrect as we should declare and initialize references at single step
```

Pointer can be reassigned, but reference can't
```c++
int a = 5;
int b = 6;
int *p;
p = &a;
p = &b;
```

```c++
int a = 5;
int b = 6;
int &p = a;
int &p = b; // This will throw an error of "multiple declaration is not allowed"

// However it is valid statement,
int &q = p;
```

Pointer has another memory address that located in stack, but reference not because it's an alias

## Source

https://www.geeksforgeeks.org/cpp/pointers-vs-references-cpp/