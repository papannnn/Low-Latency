# Custom Allocators in STL

## compile-time Minimal Allocator Interface

In order to create a minimal allocator, what you need is this

```c++
template<typename T>
struct MyAllocator {
    using value_type = T;

    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n);
};
```

Under the hood, when stl container like `vector` use the allocator, it's not directly using

```c++
alloc.allocate(n);
```

But it's actually using `allocator_traits`.

```c++
std::allocator_traits<Alloc>::allocate(alloc, n);
```

The reason `allocator_traits` created was to enable programmer to not create a lot of member in allocator class.

Before `allocator_traits`

```c++
template<class T>
struct OldAllocator {
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;

    template<class U>
    struct rebind {
        using other = OldAllocator<U>;
    };

    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n);

    void construct(T* p, const T& value);
    void destroy(T* p);
};
```

Container have to write

```c++
Alloc::pointer
Alloc::size_type
Alloc::rebind<U>::other
alloc.construct(...)
alloc.destroy(...)
```

After

```c++
template<class T>
struct MyAlloc {
    using value_type = T;

    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t n);
};
```

Because `allocator_traits` will do 

```
Container
    |
    v
allocator_traits
    |
    +--> value_type
    +--> allocate(n)
    +--> deallocate(p,n)
    |
    +--> construct()   (generated if absent)
    +--> destroy()     (generated if absent)
    +--> rebind        (generated if absent)
```

## Source

https://learn.microsoft.com/th-th/cpp/standard-library/allocators?view=msvc-160