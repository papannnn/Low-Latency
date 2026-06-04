#include <iostream>
#include "unique_ptr.hpp"

void d(int* ptr) {
    delete ptr;
    std::cout << "Deleted" << std::endl;
}

int main () {
    low_latency::unique_ptr<int> ptr(new int(6));
    
    low_latency::unique_ptr<int> ptr2 = std::move(ptr);

    low_latency::unique_ptr<int> ptr3(new int(1000));

    ptr2 = std::move(ptr3);

    std::cout << (*ptr2) << std::endl;

    if (ptr3) {
        std::cout << (*ptr3) << std::endl;
    }

    low_latency::unique_ptr<int, decltype(&d)> ptr4(new int(199), d);
}