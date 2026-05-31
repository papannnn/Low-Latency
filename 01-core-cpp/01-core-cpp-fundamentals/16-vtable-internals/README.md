# Vtable Internals

## Vptr

Assuming you have a class `Simple`

```c++
class Simple {
    int a;
    int b;

public:
    void func() {
        ///
    }
};

int main () {
    std::cout << "Size of simple: " << sizeof(Simple) << std::endl;
}
```

Output is
```
Size of simple: 8
```

We got 8 because there's 2 `int` member. `int` is 4 bytes. We don't count `func` since `func` resides in different memory location.

Now let's add `virtual` keyword to `func`

```c++
class Poly {
    int a;
    int b;

public:
    virtual void func() {
        ///
    }
};

int main () {
    std::cout << "Size of Poly: " << sizeof(Poly) << std::endl;
}
```

Output will be:
```
Size of Poly: 16
```

The reason we got +8 bytes because when you use `virtual` keyword to a class, compiler actually add a hidden member called `vptr`. `vptr` is a pointer that point to a `vtable`, because it's a pointer, the size of it is 8 bytes.

Conceptually, the vtable looks like this

```c++
void* Simple_Vtable[] = {
    &Simple::func,
    &Simple::func1
};
```

This is why we can do runtime polymorphism in C++, because `vptr` will help us to determine which function it should take.

## Drawback

Because it will do pointer chasing, that means CPU need to access memory 3 times. 

- Object to `vptr`
- Second `vptr` to `vtable`
- `vtable` to actual function

This affect CPU cycle, that means the performance will be degraded.

## Source

https://youtu.be/hS7kPtVB1vI?si=D_RJasUH0ExWTG6I

https://youtu.be/47ZP-0iBicI?si=7qgPX-EPoIHNiTrL

https://www.youtube.com/watch?v=sAZ6JvLPx_0