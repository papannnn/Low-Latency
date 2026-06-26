# Passing Arguments Safely to Threads

## Passing Arguments by Value

By default, argument are passed by value.

```c++
#include <iostream>
#include <thread>

void printSum(int a, int b) {
    std::cout << "Sum: " << a + b << std::endl;
}

int main() {
    int x = 10;
    int y = 20;

    // Pass arguments by value (default behavior)
    std::thread t(printSum, x, y);

    t.join();

    return 0;
}
```

## Passing Arguments by Reference

You can pass by reference by using `std::ref` / `std::cref`.

```c++
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>

void modifyVector(std::vector<int>& vec) {
    std::transform(vec.begin(), vec.end(), vec.begin(), [](int n) { return n * 2; });
}

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // Pass argument by reference using std::ref
    std::thread t(modifyVector, std::ref(numbers));

    t.join();

    // Display modified vector
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

## Passing Multiple Arguments

You can pass multiple argument and use pass by value, ref, const ref all together at the same time.

```c++
#include <iostream>
#include <thread>
#include <string>
#include <functional>

void complexFunction(int id, const std::string& name, std::vector<int>& data) {
    std::cout << "Thread " << id << ": Hello, " << name << "!" << std::endl;
    std::cout << "Data size: " << data.size() << std::endl;
}

int main() {
    int id = 1;
    std::string name = "Alice";
    std::vector<int> data = {1, 2, 3, 4, 5};

    // Create a thread passing multiple arguments
    std::thread t(complexFunction, id, std::cref(name), std::ref(data));

    t.join();

    return 0;
}
```

## Source

https://doincpp.in/passing-arguments-to-threads/