# NRVO

## RVO and NRVO

### RVO (Return Value Optimization)

RVO (Return Value Optimization) is a compiler optimization. 

In some cases, it allows not to create a local object that is used as a return value.

```c++
std::vector<int> GetVector()
{
  return std::vector<int>(1'000'000, 1);
}

void foo()
{
  auto vect = GetVector();
}
```

### NRVO (Named Return Value Optimization). 

This compiler optimization type is like RVO. 

Instead of creating a local return object and then moving/copying it in place of the function call, this optimization instantly creates it in the right place. 

Its difference from RVO is that NRVO is applied to lvalue objects.

```c++
std::vector<int> GetVector2()
{
  std::vector<int> result(1'000'000, 1);
  return result;
}

void foo()
{
  auto vect = GetVector();
  ....
}
```

There is also an anti-pattern that disables the use of NRVO:

```c++
typename <typename Res, typename ...T>
Res foo(T ...)
{
  Res result;
  // some calculations
  return std::move(result);
}

void bar()
{
  auto obj = foo<SomeObject>();
}
```

That's because the returned object is the result of calling the std::move function and its type is Res &&. 

The actually returned type and the type function return type according to its signature are different. 

Therefore, the compiler cannot apply NRVO for the foo function

## Source

https://pvs-studio.com/en/blog/terms/6516/