# Const Correctness

## What is const correctness

Using `const` to prevent `const` object from being mutated.

If you want to create a function `f()` that accept `std::string`, and you want to give a promise that you don't want to mutate it's object. You can do these.

```c++
void f1(const std::string& s);      // Pass by reference-to-const

void f2(const std::string* sptr);   // Pass by pointer-to-const

void f3(std::string s);             // Pass by value
```

For `f1` and `f2`, any attempt to change it's parameter will result a compilation error.

For `f3`, you can mutate it's local parameter, since you're doing a copy. That means when you change something on `s` variable, the argument that got passed doesn't change.

And if you want to give a hint that you want to change caller argument, you can do these:

```c++
void g1(std::string& s);      // Pass by reference-to-non-const

void g2(std::string* sptr);   // Pass by pointer-to-non-const
```

## What does “const X* p” mean? 

It means pointer `p` is pointing to object of class X, but pointer `p` can't change it's X object.

Read from right to left
```
p * X const

p is a pointer to X that is constant
```

That means the one cannot change is the object, you can repoint the pointer to another object.

## What’s the difference between “X* const p” and “const X* const p”?

Read from right to left
```
X * const p

p is a constant pointer to X
```

That means `p` can't be repoint to another address, but X can be changed.

Read from right to left
```
const X * const p

p is a constant pointer to X that is constant
```

That means both p and X can't be changed.

## What does “const X& x” mean?

Read from right to left
```
const X & x

x is a reference to X that is constant
```

That means you can't change `X` object using `x`.

## What is a “const member function”?

```c++
class Fred {
public:
  void inspect() const;   // This member promises NOT to change *this
  void mutate();          // This member function might change *this
};
void userCode(Fred& changeable, const Fred& unchangeable)
{
  changeable.inspect();   // Okay: doesn't change a changeable object
  changeable.mutate();    // Okay: changes a changeable object
  unchangeable.inspect(); // Okay: doesn't change an unchangeable object
  unchangeable.mutate();  // ERROR: attempt to change unchangeable object
}
```

## Source

https://isocpp.org/wiki/faq/const-correctness