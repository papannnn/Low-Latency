#include <iostream>
#include "memory_pool.hpp"

struct Data {
    ~Data() {
        std::cout << "Finished" << std::endl;
    }
};

int main () {
    MemoryPool<Data> mempool(1024);

    Data* a = mempool.allocate(Data());
    mempool.deallocate(a);
    // if (a) {
    //     std::cout << *a << std::endl;
    // }
}