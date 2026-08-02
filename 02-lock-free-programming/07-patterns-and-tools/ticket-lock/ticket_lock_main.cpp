#include <iostream>
#include "ticket_lock.hpp"
#include <thread>

int main () {
    TicketLock ticketLock;

    size_t num = 0;

    std::thread thread1([&]() {
        for (int i = 0; i < 10000000; i++) {
            ticketLock.lock();
            num++;
            ticketLock.unlock();
        }
    });

    std::thread thread2([&]() {
        for (int i = 0; i < 10000000; i++) {
            ticketLock.lock();
            num++;
            ticketLock.unlock();
        }
    });

    std::thread thread3([&]() {
        for (int i = 0; i < 10000000; i++) {
            ticketLock.lock();
            num++;
            ticketLock.unlock();
        }
    });

    std::thread thread4([&]() {
        for (int i = 0; i < 10000000; i++) {
            ticketLock.lock();
            num++;
            ticketLock.unlock();
        }
    });

    std::thread thread5([&]() {
        for (int i = 0; i < 10000000; i++) {
            ticketLock.lock();
            num++;
            ticketLock.unlock();
        }
    });

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    
    std::cout << (num == 50000000) << std::endl;
}