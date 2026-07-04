#include <thread>
#include "concurrent_vector.hpp"

int main () {
    ConcurrentVector<size_t> concurrentVector;

    std::jthread writeThread([&] () {
        for (size_t i = 0; i < 100000000000; i++) {
            concurrentVector.push(i);
        }
    });

    std::jthread readThread1([&] () {
        while (true) {
            concurrentVector.print("Thread 1");
        }
    });

    std::jthread readThread2([&] () {
        while (true) {
            concurrentVector.print("Thread 2");
        }
    });

    std::jthread readThread3([&] () {
        while (true) {
            concurrentVector.print("Thread 3");
        }
    });
}