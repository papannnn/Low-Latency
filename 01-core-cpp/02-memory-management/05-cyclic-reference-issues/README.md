# Cyclic Reference Issues

## Example

Assuming you have a class like this

```c++
class Example {
public:
    std::shared_ptr<A> ptr;
};

int main () {
    std::shared_ptr<A> ptr1(new Example);
    std::shared_ptr<A> ptr2(new Example);

    ptr1.ptr = ptr2;
    ptr2.ptr = ptr1;
}
```

That means we have this kind of graph.

```
Example1 → Example2
    ↑          │
    └──────────┘
```

That means the reference count will be

```
Example1: 2
Example2: 2
```

And if the `std::shared_ptr` got destroyed in `main` function, the object in `ptr` will not get destroyed since it's still dangling.

## Solution

The solution is to use `std::weak_ptr`

## Source

https://www.youtube.com/watch?v=4QhrvRyS2kk
