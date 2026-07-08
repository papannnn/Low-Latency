#include <condition_variable>
#include <deque>
#include <mutex>

template<typename T>
class BoundedQueue {
private:
    std::deque<T> _queue;
    std::mutex _mtx{};
    std::condition_variable _readerCv{};
    std::condition_variable _writerCv{};

    int _size;
public:
    BoundedQueue(int size): _size{size} {}

    void insert(T value) {
        std::unique_lock<std::mutex> lock(_mtx);

        if (_queue.size() >= _size) {
            _writerCv.wait(lock, [&] () {
                return _queue.size() < _size;
            });
        }

        _queue.push_back(value);
        _readerCv.notify_one();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(_mtx);

        if (_queue.size() == 0) {
            _readerCv.wait(lock, [&] () {
                return _queue.size() > 0;
            });
        }

        T val = _queue.front();
        _queue.pop_front();

        _writerCv.notify_one();

        return val;
    }
};