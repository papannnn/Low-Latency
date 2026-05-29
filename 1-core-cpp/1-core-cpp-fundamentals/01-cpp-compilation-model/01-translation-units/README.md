# Translation Units

## Stage of Compilation

In C / C++, the stage of compilation is like this

```
1. Source Code (.cpp, .hpp files)
2. Preprocessing
3. Compilation
4. Assembly
5. Linking
6. Execution 
```

## Code Example

Assuming there's 2 files, `main.cpp` & `math_utils.hpp`

`main.cpp`
```C++
#include "math_utils.h"

int main() {
    int result = add(NUMBER_TEN, 5);    //user defined function in math_utils.h header
    return 0;
}
```

`math_utils.hpp`
```C++
#define NUMBER_TEN 10
int add(int a, int b);
```

## Preprocessing

What preprocessing does, it will convert all of the `preprocessor directive` such as `#define`, `#include`, etc in `main.cpp` file.

We can do it using command
```bash
g++ -E main.cpp -o main.i
```

```C++
#include "math_utils.h"  // becomes --> int add(int a, int b);
```

## Final code after Preprocessing

This is the result after doing preprocessing (simplified)

```C++
int add(int a, int b);

int main() {
    int result = add(10, 5);
    return 0;
}
```

This is called `Translation Unit`.