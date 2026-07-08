#include <chrono>
#include <iostream>
#include <thread>
#include "bounded_queue.hpp"

int main () {
    BoundedQueue<int> q(10);

    std::jthread writer([&] () {
        int curr = 0;
        while (true) {
            q.insert(curr++);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    std::jthread reader([&] () {
        while (true) {
            int val = q.pop();
            std::cout << std::format("Consuming value: {}", val) << std::endl;
        }
    });
}