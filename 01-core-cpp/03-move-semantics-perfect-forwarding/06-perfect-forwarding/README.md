# Perfect Forwarding

## Forwarding

Forewarding is a process where one function *forward* it's parameter to another function

Perfect forwarding is not about forwarding the object, but also forward it's properties, such as constness, reference, volatile.

Assuming you have an object like this.

```c++
class Object {
 public:
  Object() = default;

  void SetName(std::string name) { name_ = std::move(name); }
  std::string GetName() const { return name_; }

 private:
  std::string name_;
};
```

You also have a function like these

```c++
void UseObject(Object &) {
  std::cout << "calling UseObject(Object &)" << std::endl;
}

void UseObject(const Object &) {
  std::cout << "calling UseObject(const Object &)" << std::endl;
}

void UseObject(Object &&) {
  std::cout << "calling UseObject(Object &&)" << std::endl;
}
```

If you do this

```c++
int main() {
  Object object;
  const Object const_object;
  UseObject(object);
  UseObject(const_object);
  UseObject(std::move(object));
}
```

Result will be 

```
calling UseObject(Object &)
calling UseObject(const Object &)
calling UseObject(Object &&)
```

Assuming you have wrapper function

```c++
template <typename T>
void NotForwardToUseObject(T x) {
  UseObject(x);
}
```

Running this code

```c++
int main() {
  Object object;
  const Object const_object;
  NotForwardToUseObject(object);
  NotForwardToUseObject(const_object);
  NotForwardToUseObject(std::move(object));
}
```

Result will be

```c++
calling UseObject(Object &)
calling UseObject(Object &)
calling UseObject(Object &)
```

We don't want this to happen.

We partially fix it by doing

```c++
template <typename T>
void HalfForwardToUseObject(T &&x) {  // universal reference
  UseObject(x);
}
```

Run it again

```c++
int main() {
  Object object;
  const Object const_object;
  HalfForwardToUseObject(object);
  HalfForwardToUseObject(const_object);
  HalfForwardToUseObject(std::move(object));
}
```

At least the constness is working, but we need to pass the rvalue reference also
```
calling UseObject(Object &)
calling UseObject(const Object &)
calling UseObject(Object &)
```

We can fix it by doing

```c++
template <typename T>
void ForwardToUseObject(T &&x) {
  UseObject(static_cast<T &&>(x));
}
```

```c++
int main() {
  Object object;
  const Object const_object;
  ForwardToUseObject(object);
  ForwardToUseObject(const_object);
  ForwardToUseObject(std::move(object));
}
```

```
calling UseObject(Object &)
calling UseObject(const Object &)
calling UseObject(Object &&)
```

It's working now.

To make it more simple, we can do
```c++
template <typename T>
void PerfectForwardToUseObject(T &&x) {
  UseObject(std::forward<T>(x));
}
```

## Source

https://levelup.gitconnected.com/perfect-forwarding-647e1caaf879