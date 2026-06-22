# `if constexpr`

## Purpose

`if constexpr` is used to allow branching in compile time. This means the code will be discarded if it's not meeting the logic

## Syntax

```c++
if constexpr (condition) {
    // Code executed only if condition is true at compile time
} else {
    // Code executed only if condition is false at compile time
}
```

Output

```
Integer Value 10
Non-Integer value:3.15
```

## Source

https://www.geeksforgeeks.org/cpp/if-constexpr-in-cpp-17/