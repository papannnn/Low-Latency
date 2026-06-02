# `new`/`delete` vs `malloc`/`free`

## Malloc

`malloc` is actually a function that wrap a system call `brk` / `mmap`

`malloc` function signature is look like this.

`void* malloc(size_t size)`

You pass the size how many you want to use it, and the function will give you the starting address.

But actually, the data more likely look like this.

```
+------------+--------------------------------+
| size       | user data                      |
+------------+--------------------------------+
```

That's why when you do `free`, you just need to pass the pointer, because they already gave some metadata for how many data it should be free.

`malloc` doesn't initialize the data when created. If you use it directly, it may give a garbage value.

`malloc` can fail, it will return `NULL` pointer.

When you create a class using malloc, it doesn't trigger constructor.

## new

It's C++ operator. But instead of taking how many bytes to allocate, it takes the type.

When you create a class using `new`, it trigger constructor.

If `new` fails to allocate memory on the heap, it will throw
`std::bad_alocation`.

## free

If you created a data using `malloc`, you need to free it when you're not using it anymore.

`free` function signature is this

`void free(void* ptr)`

## delete

Same with `new`, use `delete` to release a memory that created by `new`. 

Delete also trigger destructor of the class.

## Source

https://www.youtube.com/watch?v=b1vv-z_Otr4

https://www.youtube.com/watch?v=HPDBOhiKaD8

https://stackoverflow.com/questions/49990855/structure-of-a-malloc-block