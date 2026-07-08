#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>

class WorkQueue {
private:
    std::deque<std::function<void()>> _queue;

    std::condition_variable _cvProducer;
    std::condition_variable _cvConsumer;
    std::mutex _mtx;

    int _size;
public:
    WorkQueue(int size): _size{size} {}

    void submit(std::function<void()> f) {
        std::unique_lock<std::mutex> lock(_mtx);

        if (_queue.size() >= _size) {
            std::cout << "Producer sleep" << std::endl;
            _cvProducer.wait(lock, [&] () {
                return _queue.size() < _size;
            });
            std::cout << "Producer wakeup" << std::endl;
        }

        _queue.push_back(f);
        _cvConsumer.notify_one();
    }

    std::function<void()> pop() {
        std::unique_lock<std::mutex> lock(_mtx);

        if (_queue.size() == 0) {
            _cvConsumer.wait(lock, [&] () {
                return _queue.size() > 0;
            });
        }

        std::function<void()> res = _queue[0];
        _queue.pop_front();
        _cvProducer.notify_one();
        return res;
    }
};