#include <array>
#include <atomic>
#include <iostream>
#include <new>
#include <optional>

struct alignas(std::hardware_destructive_interference_size) PaddedAtomicBool {
    std::atomic<bool> _isTaken{};
};

template <typename T, size_t ConsumerCnt>
class RCUPtr {
private:
    alignas(std::hardware_destructive_interference_size) std::atomic<T*> _data;
    PaddedAtomicBool _takenFlag[ConsumerCnt];

    void waitReader() {
        bool isReaderStillUsing = true;
        while (isReaderStillUsing) {
            isReaderStillUsing = false;
            for (size_t i = 0; i < ConsumerCnt; i++) {
                bool flag = _takenFlag[i]._isTaken.load(std::memory_order_acquire);
                isReaderStillUsing |= flag;
            }
        }
    }

public:
    RCUPtr(): _data{nullptr} {}

    ~RCUPtr() {
        T* curr = _data.load(std::memory_order_acquire);
        delete curr;
    }
    
    RCUPtr(RCUPtr &&that) = delete;
    RCUPtr& operator=(RCUPtr &&that) = delete;
    
    RCUPtr(RCUPtr &that) = delete;
    RCUPtr& operator=(RCUPtr &that) = delete;


    void rcu_enter(size_t threadId) {
        _takenFlag[threadId]._isTaken.store(true, std::memory_order_release);
    }

    void rcu_exit(size_t threadId) {
        _takenFlag[threadId]._isTaken.store(false, std::memory_order_release);
    }

    // Need to take rcu_enter & rcu_exit after using data
    std::optional<T> data() {
        T* data = _data.load(std::memory_order_acquire);
        if (data == nullptr) {
            return std::nullopt;
        }

        return *data;
    }

    void updateData(T data) {
        T* newData = new T(std::forward<T>(data));
        T* oldData = _data.load(std::memory_order_acquire);

        if (oldData == nullptr) {
            _data.store(newData, std::memory_order_release);
            return;
        }

        _data.store(newData, std::memory_order_release);

        waitReader();

        delete oldData;
    }
};