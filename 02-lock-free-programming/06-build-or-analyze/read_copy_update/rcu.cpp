#include <atomic>
#include <iostream>
#include <thread>
#include "rcu.hpp"

int main () {
    RCUPtr<int, 3> rcu;
    std::atomic<bool> running = true;

    std::jthread producer([&] () {
        for (int i = 9999; i >= 0; i--) {
            rcu.updateData(i);
            std::cout << "Data: " << i << std::endl;
        }
        running.store(false, std::memory_order_release);
    });

    std::jthread consumer1([&] () {
        while (running.load(std::memory_order_acquire)) {
            rcu.rcu_enter(0);
            rcu.data();
            rcu.rcu_exit(0);
        }
        
    });

    std::jthread consumer2([&] () {
        while (running.load(std::memory_order_acquire)) {
            rcu.rcu_enter(1);
            rcu.data();
            rcu.rcu_exit(1);
        }
    });

    std::jthread consumer3([&] () {
        while (running.load(std::memory_order_acquire)) {
            rcu.rcu_enter(2);
            rcu.data();
            rcu.rcu_exit(2);
        }
    });
}