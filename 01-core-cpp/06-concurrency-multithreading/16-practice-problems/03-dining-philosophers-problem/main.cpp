#include <atomic>
#include <memory>
#include <thread>
#include "philosopher.hpp"

int main () {
    std::shared_ptr<std::atomic<bool>> fork1{std::make_shared<std::atomic<bool>>()};
    std::shared_ptr<std::atomic<bool>> fork2{std::make_shared<std::atomic<bool>>()};
    std::shared_ptr<std::atomic<bool>> fork3{std::make_shared<std::atomic<bool>>()};
    std::shared_ptr<std::atomic<bool>> fork4{std::make_shared<std::atomic<bool>>()};
    std::shared_ptr<std::atomic<bool>> fork5{std::make_shared<std::atomic<bool>>()};

    Philosopher philosopher1("1", fork1, fork2);
    Philosopher philosopher2("2", fork2, fork3);
    Philosopher philosopher3("3", fork3, fork4);
    Philosopher philosopher4("4", fork4, fork5);
    Philosopher philosopher5("5", fork5, fork1);

    std::jthread t1([&] () {
        while (true) {
            philosopher1.exec();
        }
    });

    std::jthread t2([&] () {
        while (true) {
            philosopher2.exec();
        }
    });

    std::jthread t3([&] () {
        while (true) {
            philosopher3.exec();
        }
    });

    std::jthread t4([&] () {
        while (true) {
            philosopher4.exec();
        }
    });

    std::jthread t5([&] () {
        while (true) {
            philosopher5.exec();
        }
    });
}