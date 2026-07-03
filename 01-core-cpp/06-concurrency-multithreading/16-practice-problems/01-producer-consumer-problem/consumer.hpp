#pragma once

#include <iostream>
#include "queue.hpp"

class Consumer {
private:
    Queue* _queue;

    bool* _isShutdown = nullptr;

public:
    Consumer(Queue* queue, bool* isShutdown) : _queue{queue}, _isShutdown{isShutdown} {}

    void exec() {
        std::cout << "Starting Consumer" << std::endl;

        while (!(*_isShutdown)) {
            size_t val = _queue->pop();
            std::cout << "Consuming value: " << val << std::endl;
        }

        std::cout << "Closing Consumer" << std::endl;
    }
};