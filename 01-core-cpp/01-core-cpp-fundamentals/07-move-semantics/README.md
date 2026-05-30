# Move Semantics

## Why use move semantics

Copying value is expensive, especially when the data is big.

```c++
#include <iostream>
#include <string>

int main() {
    std::string original = "Hello, World!";

    // Copying the string
    std::string copy = original;

    // Moving the string
    std::string moved = std::move(original);

    // Output all three
    std::cout << "Original: '" << original << "'" << std::endl;
    std::cout << "Copy:     '" << copy << "'" << std::endl;
    std::cout << "Moved:    '" << moved << "'" << std::endl;

    return 0;
}
```

```bash
Original: ''
Copy:     'Hello, World!'
Moved:    'Hello, World!'
```

## Source

https://www.geeksforgeeks.org/cpp/stdmove-in-utility-in-c-move-semantics-move-constructors-and-move-assignment-operators/