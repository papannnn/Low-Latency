#pragma once

#include <vector>
#include <atomic>
#include <mutex>

class Queue {
private:
    std::mutex _mtx{};
    std::condition_variable* _conditionVarConsumer{};
    std::condition_variable* _conditionVarProducer{};
    
    std::vector<size_t> _arr{};
    size_t _currIdx{};
    size_t _consumedIdx{};
    size_t _size{};

    bool* _isShutdown = nullptr;

public:
    Queue(size_t size, std::condition_variable* conditionVarConsumer, 
        std::condition_variable* conditionVarProducer, bool* isShutdown) : 
        _arr{size}, _size(size), _conditionVarConsumer{conditionVarConsumer},
        _conditionVarProducer{conditionVarProducer}, _isShutdown{isShutdown} {}

    void push(size_t ele) {
        std::unique_lock lock(_mtx);

        _conditionVarProducer->wait(lock, [&] () {
            return _currIdx - _consumedIdx < _size || *_isShutdown;
        });

        _arr[_currIdx % _size] = std::move(ele);
        _currIdx++;
        _conditionVarConsumer->notify_one();
    }

    size_t pop() {
        std::unique_lock lock(_mtx);

        _conditionVarConsumer->wait(lock, [&] () {
            return _currIdx > _consumedIdx || *_isShutdown;
        });

        size_t res = _arr[_consumedIdx % _size];
        _consumedIdx++;
        _conditionVarProducer->notify_one();
        return res;
    }
};