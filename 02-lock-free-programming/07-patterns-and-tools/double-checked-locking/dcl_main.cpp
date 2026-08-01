#include <chrono>
#include "dcl.hpp"
#include <thread>

int main () {
    std::jthread t1([] () {
        Singleton* singleton = Singleton::getInstance();
        singleton->doSomething();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    });

    std::jthread t2([] () {
        Singleton* singleton = Singleton::getInstance();
        singleton->doSomething();
        std::this_thread::sleep_for(std::chrono::seconds(2));
    });
}
