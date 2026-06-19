# Class Templates

## Usecase

Assuming you want to create a class that stores array int

```c++
class ArrayInt {
public:
    int arr[10];

    void fill(int value) {
        for (int i = 0 ; i < 10; i++) {
            arr[i] = value;
        }
    }

    int& at(int idx) {
        return arr[idx];
    }
};
```

Now assuming you also want to have ArrayDouble class

```c++
class ArrayInt {
public:
    int arr[10];

    void fill(int value) {
        for (int i = 0 ; i < 10; i++) {
            arr[i] = value;
        }
    }

    int& at(int idx) {
        return arr[idx];
    }
};

class ArrayDouble {
public:
    double arr[10];

    void fill(double value) {
        for (int i = 0 ; i < 10; i++) {
            arr[i] = value;
        }
    }

    double& at(int idx) {
        return arr[idx];
    }
};
```

It's starting to get messy, if we have 20 different type, that means a lot of duplication and a lot of maintaining code.

We can solve this using template

```c++
template <typename T>
class Array {
public:
    T arr[10];

    void fill(T value) {
        for (int i = 0 ; i < 10; i++) {
            arr[i] = value;
        }
    }

    T& at(int idx) {
        return arr[idx];
    }
};
```

To use it, just do this

```c++
int main () {
    Array<int> arrObjInt;
    Array<double> arrObjDouble;
}
```

## Source

https://www.youtube.com/watch?v=mQqzP9EWu58