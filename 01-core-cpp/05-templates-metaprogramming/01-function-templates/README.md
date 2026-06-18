# Function Templates

## Topics

Assuming you want to create a function to check which variable is larger

```c++
double getMax(double a, double b) {
    return a > b ? a : b;
}
```

Now let's assume I want another `getMax`, but this time for `int`, we can do parameter overload

```c++
double getMax(double a, double b) {
    return a > b ? a : b;
}

int getMax(int a, int b) {
    return a > b ? a : b;
}
```

Done, but now it's getting messy. What if we want to have 20 different type? Not only we need to create it, but also maintain and change all of that if we want to add some extra logic.

We can solve this using template.

```c++
template <typename T>
T getMax(T a, T b) {
    return a > b ? a : b;
}
```

Now it's solved.

`T` in

```c++
template <typename T> 
```

Is actually a template parameter. It allow us to specify one or multiple template parameter.

Under the hood, compiler is actually generated the code at compile time.

So if you do something like this

```c++
int main () {
    int a, b;
    double c, d;
    getMax(a, b);
    getMax(c, d);
}
```

At compile time, the compiler generates

```c++
double getMax(double a, double b) {
    return a > b ? a : b;
}

int getMax(int a, int b) {
    return a > b ? a : b;
}
```

## Source

https://www.geeksforgeeks.org/cpp/templates-cpp/

https://ryonaldteofilo.medium.com/template-type-deduction-in-c-3daa64b09bbb

https://www.youtube.com/watch?v=XgGEJylsWsQ