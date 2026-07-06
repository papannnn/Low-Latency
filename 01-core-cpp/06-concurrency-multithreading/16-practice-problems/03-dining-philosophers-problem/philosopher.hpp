#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

class Philosopher {
private:
    std::string _name;
    std::shared_ptr<std::atomic<bool>> _left;
    std::shared_ptr<std::atomic<bool>> _right;

public:
    Philosopher(
        std::string name,
        std::shared_ptr<std::atomic<bool>> &left,
        std::shared_ptr<std::atomic<bool>> &right
    ): _name{name}, _left{left}, _right{right} {}

    void exec() {
        std::cout << "Philosopher " << _name << " is thinking..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        bool expected{};

        std::cout << "Philosopher " << _name << " is trying to take left fork" << std::endl;
        if (!_left->compare_exchange_weak(
            expected, 
            true, 
            std::memory_order_acq_rel, 
            std::memory_order_acquire
        )) {
            std::cout << "Philosopher " << _name << " failed taking left fork" << std::endl;
            return;
        }

        std::cout << "Philosopher " << _name << " successfully taking left fork" << std::endl;
        std::cout << "Philosopher " << _name << " is trying to take right fork" << std::endl;
        expected = false;
        if (!_right->compare_exchange_weak(
            expected, 
            true, 
            std::memory_order_acq_rel, 
            std::memory_order_acquire
        )) {
            std::cout << "Philosopher " << _name << " failed taking right fork" << std::endl;
            _left->store(false, std::memory_order_release);
            return;
        }

        std::cout << "Philosopher " << _name << " successfully taking right fork" << std::endl;
        std::cout << "Philosopher " << _name << " is eating" << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Philosopher " << _name << " putting both fork" << std::endl;

        _left->store(false, std::memory_order_release);
        _right->store(false, std::memory_order_release);
    }
};