#include <iostream>
#include "shared_ptr.hpp"

int main () {
    low_latency::shared_ptr<int> ptr(new int(100));
    if (ptr) {
        std::cout << *(ptr) << std::endl;
    }

    low_latency::shared_ptr<int> ptr2 = ptr;
    if (ptr2) {
        std::cout << *(ptr2) << std::endl;
    }

    *ptr = 200;
    if (ptr2) {
        std::cout << *(ptr2) << std::endl;
    }

    low_latency::shared_ptr<int> ptr3 = std::move(ptr2);
}