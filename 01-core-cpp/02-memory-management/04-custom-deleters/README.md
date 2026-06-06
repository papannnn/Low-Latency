# Custom Deleters

## Example Code

```c++
#include <cstdio>
#include <functional>
#include <memory>
#include <iostream>

// Helper function for the custom deleter for C read/write files.
// Remember, C does not have constructor and destructor.
// Custom objects will need custom initialization and free functions,
// especially when memory is allocated on heap.
// In modern C++, we read/write files using fstream
// https://en.cppreference.com/w/cpp/header/fstream
// std::fstream has a destructor and there is no need to use custom deleter.
void close_file(std::FILE* fp) { std::fclose(fp); }

// Helper functor for the custom deleter for C read/write files.
struct FileCloser
{
    void operator()(std::FILE* fp) const { std::fclose(fp); }
};

// A C++ class.
struct Foo
{
    Foo() {}
    ~Foo() {}
};

// The customer deleter is often not meaningful for C++ objects
// as C++ objects has their own destructors.
struct FooDeleter
{
    void operator()(Foo* p) const { delete p; }
};

int main()
{

    FileCloser fcloser;
    std::FILE* ptr_file = std::fopen("file.txt", "w");

    // std::unique_ptr requires deleter as part of the type.
    // std::unique_ptr can be a zero-overhead abstraction with some optimization
    // compared with raw pointer, provided that the deleter is stateless.
    // Templated class.
    // template<class T, class Deleter> std::unique_ptr
    // // Function pointer
    std::unique_ptr<std::FILE, decltype(&close_file)> unique_ptr_file(
        ptr_file, &close_file);
    // // Functor
    // std::unique_ptr<std::FILE, FileCloser> unique_ptr_file(ptr_file,
    // fcloser);
    // // Lambda expression.
    // std::unique_ptr<std::FILE, std::function<void(std::FILE*)>>
    // unique_ptr_file(ptr_file, [](std::FILE* fp) { std::fclose(fp); });

    if (unique_ptr_file)
    {
        std::fputs("fopen example", unique_ptr_file.get());
    }

    // No need to call std::fclose.

    // Templated constructor.
    // template<class Y, class Deleter> shared_ptr(Y *ptr, Deleter d)
    // The use of customer deleter for Foo is not meaningful here,
    // as Foo already has a destructor.
    std::shared_ptr<Foo> shared_ptr_foo_1(new Foo(), FooDeleter());
    // Constructor does not allow new deleter for an existing shared_ptr.
    // Can't have two deleters for the same object.
    // std::shared_ptr<Foo> shared_ptr_foo_2(shared_ptr_foo_1, FooDeleter());
    std::shared_ptr<Foo> shared_ptr_foo_2(shared_ptr_foo_1);

    std::cout << sizeof(std::FILE*) << std::endl; // 8 Bytes
    std::cout << sizeof(std::unique_ptr<std::FILE>) << std::endl; // 8 Bytes
    std::cout << sizeof(std::unique_ptr<std::FILE, FileCloser>) << std::endl; // 8 Bytes
    std::cout << sizeof(std::unique_ptr<std::FILE, decltype(&close_file)>) << std::endl; // 16 Bytes
    std::cout << sizeof(std::unique_ptr<std::FILE, std::function<void(std::FILE*)>>) << std::endl; // 40 Bytes
    std::cout << sizeof(std::shared_ptr<Foo>) << std::endl; // 16 Bytes

    return 0;
}
```

## EBO

As you can see, if the deleter is from Empty & Inheritable class, the size of the unique_ptr will be 8 bytes. It's because it uses EBO (Empty Base Optimization)

## Reason

The reason you want to use custom deleter because maybe you want more than releasing the data in pointer, for example, closing file descriptor.

## Source

https://leimao.github.io/blog/CPP-Smart-Pointer-Custom-Deleter/
