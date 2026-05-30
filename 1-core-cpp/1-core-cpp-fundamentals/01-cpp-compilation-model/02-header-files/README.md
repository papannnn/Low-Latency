# Header Files

## Reusable declaration

Assuming you have 2 `.cpp` files, `main.cpp`, and `logger.cpp`

`main.cpp`
```C++
#include <iostream>
void Log(const char* message) {
    std::cout << message << std::endl;
}

int main () {
    Log("Hello World");
}
```

`logger.cpp`
```C++
void InitLog() {
    Log("Initializing Log"); // Compilation error, no Log function got declared
}
```

`Log` function is actually fine is `main.cpp` because it's already being declared there.

But, `logger.cpp` didn't aware of `Log` function yet.

If we compile `main.cpp`

```bash
g++ -c main.cpp -o main.o
```

It works fine.

But if we compile `Logger.cpp`

```bash
g++ -c logger.cpp -o logger.o
```

```bash
logger.cpp:2:5: error: use of undeclared identifier 'Log'
    2 |     Log("Initializing Log"); // Compilation error, no Log function got declared
      |     ^
1 error generated.
```

Now how do we tell `logger.cpp` that `Log` function is actually exist?

We can just actually put a function declaration inside `logger.cpp`

`logger.cpp`
```C++
void Log(const char* message);

void InitLog() {
    Log("Initializing Log");
}
```

Now, when you compile `logger.cpp` again, it would be fine now.

```bash
g++ -c logger.cpp -o logger.o
```

Now let's create another file, called `server.cpp`, and we want to use `Log` function as well. Do we need to create function declaration inside `server.cpp`?

The answer is yes, but we have a better way to do it.

**Header Files**

We create a file called `logger.hpp`

`logger.hpp`
```C++
#pragma once

void Log(const char* message)
```

Then, in `server.cpp` and `logger.cpp`, we use that header file.

`logger.cpp`
```C++
#include "logger.hpp"

void InitLog() {
    Log("Initializing Log");
}
```

Assuming `main.cpp` want to use `InitLog` function from `logger.cpp`, we can put `InitLog` function declaration inside `logger.hpp` header files.

`logger.hpp`
```C++
#pragma once

void Log(const char* message)
void InitLog();
```

`main.cpp`
```C++
#include <iostream>
#include <logger.hpp>

void Log(const char* message) {
    std::cout << message << std::endl;
}

int main () {
    InitLog();
    Log("Hello World");
}
```

Now, if we make `main.cpp` & `logger.cpp` into executable file

```bash
g++ logger.cpp main.cpp
./a.out
```

It will printout on the console
```
Initializing Log
Hello World
```

You may notice I'm adding 
```
#pragma once
``` 
inside the `logger.hpp` file.

What it does it will act as a header guard. So it will prevent function declaration being called more than once.

## Source

https://www.youtube.com/watch?v=9RJTQmK0YPI