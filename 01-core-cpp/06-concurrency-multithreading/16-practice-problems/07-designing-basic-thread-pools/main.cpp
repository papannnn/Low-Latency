#include <format>
#include <functional>
#include <iostream>
#include <thread>
#include "work_queue.hpp"

int main () {
    WorkQueue workQueue(5);

    std::jthread producer([&]() {
        int i = 0;
        while (true) {
            workQueue.submit([=] () {
                std::cout << std::format("Doing {} work", i) << std::endl;
            });
            i++;
        }
    });

    std::jthread consumer1([&]() {
        while (true) {
            std::function<void()> f = workQueue.pop();
            f();
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    });

    std::jthread consumer2([&]() {
        while (true) {
            std::function<void()> f = workQueue.pop();
            f();
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    });

    std::jthread consumer3([&]() {
        while (true) {
            std::function<void()> f = workQueue.pop();
            f();
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    });

    std::jthread consumer4([&]() {
        while (true) {
            std::function<void()> f = workQueue.pop();
            f();
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    });
}