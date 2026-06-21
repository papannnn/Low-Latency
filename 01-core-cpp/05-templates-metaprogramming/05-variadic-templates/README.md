# Variadic Templates

## Example

You can make a variadic function templates by doing it like this

```c++
// C++ program to demonstrate working of
// Variadic function Template
#include <iostream>
using namespace std;

// To handle base case of below recursive
// Variadic function Template
void print()
{
    cout << "I am empty function and "
            "I am called at last.\n";
}

// Variadic function Template that takes
// variable number of arguments and prints
// all of them.
template <typename T, typename... Types>
void print(T var1, Types... var2)
{
    cout << var1 << endl;

    print(var2...);
}

// Driver code
int main()
{
    print(1, 2, 3.14,
          "Pass me any "
          "number of arguments",
          "I will print\n");

    return 0;
}
```

The result will be

```c++
1
2
3.14
Pass me any number of arguments
I will print

I am empty function and I am called at last.
```

## How it works

```c++
cout<< 1 <<endl ;
print(2, 3.14, "Pass me any number of arguments", 
      "I will print\n");
```

```c++
cout<< 2 <<endl ;
print(3.14, "Pass me any number of arguments", 
      "I will print\n");
```

```c++
cout<< 3.14 <<endl ;
print("Pass me any number of arguments", 
      "I will print\n");
```

```c++
cout<< "Pass me any number of arguments" <<endl ;
print("I will print\n");
```

```c++
cout<< "I will print\n" <<endl ;
print();
```

## Fold expression

As you can see, you need to use

```c++
void print(T var1, Types... var2);
```

and 

```c++
void print();
```

This actually kinda redundant.

In C++17, we can use fold expression

```c++
template <typename... Types>
void print(Types... var) {
    ((cout << var << endl), ...);
}
```

If you call this

```c++
print(1, 2, 3.14,
          "Pass me any "
          "number of arguments",
          "I will print\n");
```

It will produces

```
1
2
3.14
Pass me any number of arguments
I will print

```

Because this actually expands to

```c++
(std::cout << 1 << '\n'),
(std::cout << 2 << '\n'),
(std::cout << 3.14 << '\n'),
(std::cout << "Pass me any number of arguments" << '\n'),
(std::cout << "I will print\n" << '\n');
```
## Source

https://www.geeksforgeeks.org/cpp/variadic-function-templates-c/

https://www.youtube.com/watch?v=9D8uPKDffdI

https://www.youtube.com/watch?v=HdG0FtJfwkU

https://youtu.be/HdG0FtJfwkU?si=1_N7fyb32VDLrXIp