#include "barber.hpp"
#include <thread>

int main () {
    Barber barber;

    std::jthread tBarber([&] () {
        while (true) {
            barber.exec();
        }
    });

    std::jthread t1([&] () {
        // while (true) {
            barber.queue(1);
        // }
    }); 

    std::jthread t2([&] () {
        // while (true) {
            barber.queue(2);
        // }
    });

    std::jthread t3([&] () {
        // while (true) {
            barber.queue(3);
        // }
    });

    std::jthread t4([&] () {
        // while (true) {
            barber.queue(4);
        // }
    });

    std::jthread t5([&] () {
        // while (true) {
            barber.queue(5);
        // }
    });
}