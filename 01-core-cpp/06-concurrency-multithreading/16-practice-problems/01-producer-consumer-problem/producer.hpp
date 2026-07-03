#pragma once

#include <iostream>
#include "queue.hpp"

class Producer {
private:
    Queue* _queue;
    bool* _isShutdown = nullptr;

    size_t _val{};
public:
    Producer(Queue* queue, bool* isShutDown) : _queue{queue}, _isShutdown{isShutDown} {}

    void exec() {
        std::cout << "Starting Producer" << std::endl;

        while (!(*_isShutdown)) {
            _queue->push(++_val);
            std::cout << "Producer generated value: " << _val << std::endl;
        }
        
        std::cout << "Closing Producer" << std::endl;
    }
};