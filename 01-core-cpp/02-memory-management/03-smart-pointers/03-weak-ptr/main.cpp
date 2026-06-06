#include <iostream>
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

int main () {
    low_latency::shared_ptr<int> ptr(new int(100));

    if (ptr) {
        std::cout << *ptr << std::endl;
        std::cout << ptr.use_count() << std::endl;
    }

    low_latency::shared_ptr<int> ptr2 = ptr;
    std::cout << ptr.use_count() << std::endl;
    *ptr = 200;
    std::cout << *ptr2 << std::endl;

    low_latency::shared_ptr<int> ptr3(new int(999), [](int* ptr) {
        std::cout << "HELLOOOOO" << std::endl;
        delete ptr;
    });

    low_latency::weak_ptr<int> ptr4 = ptr3;
    std::cout << "Hey " << *(ptr4.lock()) << std::endl;
}