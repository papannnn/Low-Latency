#include "hazard_pointer.hpp"
#include <iostream>
#include <thread>

int main () {
    HazardPointer<5, int> hazardPointer;

    int* val = new int(10);

    std::jthread thread1([&]() {
        HazardPointerOwner owner = hazardPointer.make_owner();
        int* temp = val;
        owner.protect(temp);
        if (temp == val) {
            std::cout << "Valid" << std::endl;
        } else {
            std::cout << "Value changed, need to refetch" << std::endl;
        }
        owner.unprotect(temp);
        val = new int(20);
        owner.retire(temp);
    });

    std::jthread thread2([&]() {
        HazardPointerOwner owner = hazardPointer.make_owner();
        int* temp = val;
        owner.protect(temp);
        if (temp == val) {
            std::cout << "Valid" << std::endl;
        } else {
            std::cout << "Value changed, need to refetch" << std::endl;
        }
        val = new int(20);
        owner.retire(temp);
    });
}