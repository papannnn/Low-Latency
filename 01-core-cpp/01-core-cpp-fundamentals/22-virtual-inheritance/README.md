# Virtual Inheritance

## Problem

Let's see this example code

```c++
class Base {
    int base_value = 42;
};

class Left : public Base {
    int i = 1;
};

class Right : public Base {
    int i = 2;
};

class Derived : public Left, public Right {

};

int main () {
    Derived d;
}
```

It will look like this

```
        Base #1                Base #2
   (base_value = 42)      (base_value = 42)
          ▲                      ▲
          │                      │
       Left                   Right
      (i = 1)                (i = 2)
          ▲                      ▲
          └─────────┬────────────┘
                    │
                 Derived
```

As you can see, it's actually having 2 distinct Base object.

We don't want that.

```c++
int main () {
    Derived d;
    d.Left::base_value++;
    d.Right::base_value++;

    std::cout << d.Left::base_value << std::endl;
    std::cout << d.Right::base_value << std::endl;
}
```

The output will be
```
43
43
```

To fix this, we need to make sure that Base is only 1 instance.

```c++
class Base {
    int base_value = 42;
};

class Left : virtual public Base {
    int i = 1;
};

class Right : virtual public Base {
    int i = 2;
};

class Derived : public Left, public Right {

};

int main () {
    Derived d;
}
```

Now, the representation will look like this

```
                 Base
          (base_value = 42)
                 ▲
                 │
         virtual │ virtual
                 │
          +------+------+
          |             |
          |             |
        Left          Right
       (i = 1)       (i = 2)
          ▲             ▲
          └──────┬──────┘
                 │
              Derived
```

## Source

https://www.youtube.com/watch?v=vZPkYvsqQxQ