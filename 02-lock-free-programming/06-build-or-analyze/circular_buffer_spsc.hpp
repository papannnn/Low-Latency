#include <atomic>
#include <new>
#include <optional>
#include <utility>

template <typename T>
class CircularBuffer {
private:
    size_t _capacity;
    T* _arr;
    alignas(std::hardware_destructive_interference_size) std::atomic<size_t> _popPointer{};
    alignas(std::hardware_destructive_interference_size) std::atomic<size_t> _pushPointer{};

    // Exclusive for producer thread
    alignas(std::hardware_destructive_interference_size) size_t _cachedPopPointer{};
    // Exclusive for consumer thread
    alignas(std::hardware_destructive_interference_size) size_t _cachedPushPointer{};
public:
    CircularBuffer(size_t capacity): _capacity{std::move(capacity)}, _arr{static_cast<T*>(malloc(_capacity * sizeof(T)))} {}

    CircularBuffer(const CircularBuffer &circullarBuffer) = delete;
    CircularBuffer& operator=(const CircularBuffer &circullarBuffer) = delete;

    ~CircularBuffer() {
        while (!isEmpty(_pushPointer.load(std::memory_order_relaxed), _popPointer.load(std::memory_order_relaxed))) {
            pop();
        }
        free(_arr);
    }

    bool isEmpty(size_t pushPointer, size_t popPointer) {
        return pushPointer == popPointer;
    }

    bool isFull(size_t pushPointer, size_t popPointer) {
        return pushPointer - popPointer == _capacity;
    }

    bool push(T val) {
        size_t pushPointer = _pushPointer.load(std::memory_order_acquire);
        if (isFull(pushPointer, _cachedPopPointer)) {
            _cachedPopPointer = _popPointer.load(std::memory_order_acquire);
            if (isFull(pushPointer, _cachedPopPointer)) {
                return false;
            }
        }

        new (&_arr[pushPointer % _capacity]) T(std::move(val));
        _pushPointer.store(pushPointer + 1, std::memory_order_release);
        return true;
    }

    std::optional<T> pop() {
        size_t popPointer = _popPointer.load(std::memory_order_acquire);
        if (isEmpty(_cachedPushPointer, popPointer)) {
            _cachedPushPointer = _pushPointer.load(std::memory_order_acquire);
            if (isEmpty(_cachedPushPointer, popPointer)) {
                return std::nullopt;
            }
        }

        size_t idx = popPointer % _capacity;
        T retVal = std::move(_arr[idx]);
        _arr[idx].~T();
        _popPointer.store(popPointer + 1, std::memory_order_release);
        return retVal;
    }
};