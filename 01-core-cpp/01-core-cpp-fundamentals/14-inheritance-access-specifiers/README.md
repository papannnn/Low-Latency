# Inheritance: Public, Private, Protected

## Type of Inheritance Access

### Public

Base public     -> Child public

Base protected  -> Child protected

Base private    -> inaccessible

### Protected

Base public     -> Child protected

Base protected  -> Child protected

Base private    -> inaccessible

### Private

Base public     -> Child private

Base protected  -> Child private

Base private    -> inaccessible


### Tldr

Find which one is more stricter, that will be the child accessor.

## Source

https://www.geeksforgeeks.org/cpp/cpp-inheritance-access/
