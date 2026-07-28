#include "free_list.hpp"
#include <iostream>

class Dummy {
    size_t a;
    size_t b;
    size_t c;
    size_t d;
};

int main () {
    FreeList freeList(1024);
    freeList.debug();
    int* a = freeList.allocate<int>(50);
    freeList.debug();

    int* b = freeList.allocate<int>(55);
    freeList.debug();

    freeList.deallocate(a);

    freeList.debug();

    freeList.deallocate(b);
    freeList.debug();
}