#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include "seq_lock.hpp"

int main () {
    SeqLockData<size_t> seqLock;
    std::atomic<bool> finished = false;

    std::jthread prod([&] () {
        size_t i = 0;
        while (i < 5) {
            seqLock.store(i);
            i++;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        finished.store(true, std::memory_order_release);
    });

    std::jthread consumer1([&] () {
        while (!finished.load(std::memory_order_acquire)) {
            size_t val = seqLock.load();
            std::cout << val << std::endl;
        }
    });

    std::jthread consumer2([&] () {
        while (!finished.load(std::memory_order_acquire)) {
            size_t val = seqLock.load();
            std::cout << val << std::endl;
        }
    });
    // std::cout << "Finished" << std::endl;
}