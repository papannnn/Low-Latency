#pragma once

#include <atomic>
#include <iostream>
#include <new>
#include <utility>

template <typename T>
class SeqLockData {
private:
    alignas(std::hardware_destructive_interference_size) T _data;
    alignas(std::hardware_destructive_interference_size) std::atomic<size_t> _seq{0};
public:

    T load() {
        T data;
        while (true) {
            size_t seq = _seq.load(std::memory_order_acquire);
            bool isWriting = seq % 2 != 0;
            if (isWriting) {
                continue;
            }

            data = _data;

            if (seq == _seq.load(std::memory_order_acquire)) {
                break;
            }
        }
        return data;
    }

    void store(T data) {
        size_t seq = _seq.load(std::memory_order_relaxed);
        _seq.store(seq + 1, std::memory_order_release);

        std::atomic_signal_fence(std::memory_order_acq_rel);

        _data = std::move(data);

        std::atomic_signal_fence(std::memory_order_acq_rel);

        _seq.store(seq + 2, std::memory_order_release);
    }
};