# Placement `new`

- Constructing objects in existing storage
- Manual lifetime management
- Alignment requirements
- Explicit destructor calls
- Common placement `new` use cases

## Example Code

```c++
#include <iostream>

class Example {
public:
    std::string name;

    Example(std::string name_) : name(std::move(name_) + " example very long string in here hello hey hello i don't want SSO") {}

    ~Example() {
        std::cout << name << " is destroyed" << std::endl;
    }
};

int main () {
    alignas(Example) char buffer[1024];

    std::cout << "Size: " << sizeof(Example) << std::endl;
    std::cout << "Alignment: " << alignof(Example) << std::endl;
    Example* buf = reinterpret_cast<Example*>(buffer);

    std::cout << "address as integer: "
              << reinterpret_cast<std::uintptr_t>(buffer)
              << '\n';

    for (size_t i = 0; i < 1024 / sizeof(Example); i++) {
        new (buf + i) Example(std::to_string(i));
    }

    for (size_t i = 0; i < 1024 / sizeof(Example); i++) {
        std::cout << (buf + i)->name << std::endl;
    }

    for (size_t i = 0; i < 1024 / sizeof(Example); i++) {
        (buf + i)->~Example();
    }

    // UB Example
    for (size_t i = 0; i < 1024 / sizeof(Example); i++) {
        std::cout << (buf + i)->name << std::endl;
    }
}
```

## Source

https://www.youtube.com/watch?v=2bsGFQgBMXs