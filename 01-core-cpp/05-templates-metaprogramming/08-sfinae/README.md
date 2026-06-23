# SFINAE

## Goal

Is for intended implementation is selected over the alternatives which should be incompatible.

With template, we can exploit error using:

- Parameter list
- Return type
- Argument

## Example 1

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

Because creating array can't be arr[0]. Array size can't be zero.

We put nullptr because we actually don't care about it, we make this to trigger "compilation error".

When there's something error when compiler try to generate the code, the compiler will try to find another candidate to try it again.

## Example 2 

```c++
class RobotRocket {
    static int Bullets{};

    void shootBullets() {
        //
    }
};

class RobotMissile {
    static int Missiles{};

    void shootMissiles() {
        //
    }
}

template <typename Robot>
void handleEnemies(Robot r, decltype(Robot::Bullets)* = nullptr) {
    r.shootBullets();
}

template <typename Robot>
void handleEnemies(Robot r, decltype(Robot::Missiles)* = nullptr) {
    r.shootMissiles();
}
```

Like previous example, we take advantage of subtitution of function parameter.

If one function is failing, it will try another function.

## Example 3

```c++
struct SensorA {
    struct IpV4 {
        int address{};
    };

    IpV4 ipV4;
};

template <typename T>
typename S::IpV4 getAddress(S sensor) {
    return return sensor.ipV4;
}

template <typename T>
typename S::IpV6 getAddress(S sensor) { // Fail to compile
    return return sensor.ipV6;
}

int main () {
    SensorA sensor;
    getAddress(sensor);
}
```

## Source

https://youtu.be/-Z7EOWVkb3M?si=GtvPN0wKJgxfl6gG