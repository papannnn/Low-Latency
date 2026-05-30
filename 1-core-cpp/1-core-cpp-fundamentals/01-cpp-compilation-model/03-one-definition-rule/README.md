# One Definition Rule

## Definition

You can't have multiple definition for variable, function, class, or object.

In one translation unit, you can have multiple declaration, but can't have multiple definition.

For example, assuming you have `logger.cpp` file.

`logger.cpp`
```c++
void InitLog();
void InitLog();
void InitLog();
```

This is valid and can be compiled
```bash
g++ -c logger.cpp
```

But if you do
```c++
void InitLog();
void InitLog();

void InitLog() {
    Log("Initializing Log");
}

void InitLog() {
    Log("Initializing Log");
}
```

It will throw compilation error
```bash
logger.cpp:11:6: error: redefinition of 'InitLog'
   11 | void InitLog() {
      |      ^
logger.cpp:7:6: note: previous definition is here
    7 | void InitLog() {
      |      ^
1 error generated.
```

## Source
https://jitpaul.blog/2017/07/13/concepts-you-should-know-as-a-c-programmer/