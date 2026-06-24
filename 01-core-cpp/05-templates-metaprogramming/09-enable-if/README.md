# `enable_if`

## Usage

Enable if is used for SFINAE, instead doing this that looks dirty

```c++
template <typename T, int I>
bool areEqualImpl(T a, T b, char (*f)[I == 0] = nullptr) {
    return a == b;
}

template <typename T, int I>
bool areEqualImpl(T a, T b, char (*f)[I == 1] = nullptr) {
    return std::abs(a - b) < 0.1;
}

template <typename T>
bool areEqual(T a, T b) {
    constexpr int isFloating = std::is_floating_point_v<T> ? 1 : 0;
    return areEqualImpl<T, isFloating>(a, b);
}
```

You can just do this

```c++
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
bool areEqualImpl(T a, T b) {
    return a == b;
}

template <typename T, typename = std::enable_if_t<!std::is_floating_point_v<T>>>
bool areEqualImpl(T a, T b) {
    return std::abs(a - b) < 0.1;
}

template <typename T>
bool areEqual(T a, T b) {
    return areEqualImpl<T>(a, b);
}
```

## Source

https://youtu.be/-Z7EOWVkb3M?si=7Xe0GyKCmKsc528_