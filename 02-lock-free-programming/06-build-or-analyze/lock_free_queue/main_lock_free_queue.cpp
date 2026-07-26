#include <chrono>
#include <iostream>
#include <optional>
#include "lock_free_queue.hpp"
#include <thread>

int main () {
    LockFreeQueue<size_t> q;

    std::jthread t1([&] () {
        size_t i = 0;
        while (true) {
            q.push(i++);
        }
    });

    std::jthread t2([&] () {
        while (true) {
            std::optional<size_t> val = q.pop();
            if (val) {
                std::cout << "Printing: " << val.value() << std::endl;
            } else {
                std::cout << "Not getting value" << std::endl;
            }
        }
    });
}