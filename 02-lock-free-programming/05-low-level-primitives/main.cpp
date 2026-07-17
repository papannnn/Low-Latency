#include <iostream>
#include "spinlock.hpp"
#include <thread>

int main () {
    SpinLock spinLock;
    std::jthread t1([&] () {
        for (int i = 0 ; i < 1000; i++) {
            spinLock.lock();
            std::cout << "Thread 1 cnt: " << i << std::endl;
            spinLock.unlock();
        }
    });

    std::jthread t2([&] () {
        for (int i = 0 ; i < 1000; i++) {
            spinLock.lock();
            std::cout << "Thread 2 cnt: " << i << std::endl;
            spinLock.unlock();
        }
    });

    std::jthread t3([&] () {
        for (int i = 0 ; i < 1000; i++) {
            spinLock.lock();
            std::cout << "Thread 3 cnt: " << i << std::endl;
            spinLock.unlock();
        }
    });

    std::jthread t4([&] () {
        for (int i = 0 ; i < 1000; i++) {
            spinLock.lock();
            std::cout << "Thread 4 cnt: " << i << std::endl;
            spinLock.unlock();
        }
    });

    std::jthread t5([&] () {
        for (int i = 0 ; i < 10000; i++) {
            spinLock.lock();
            std::cout << "Thread 5 cnt: " << i << std::endl;
            spinLock.unlock();
        }
    });
}