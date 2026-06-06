#pragma once

#include <atomic>

namespace low_latency {

template<typename T>
struct DefaultDeleter {
    void operator()(T* ptr) {
        delete ptr;
    }
};

class control_block_base {
    std::atomic<size_t> strong_cnt;
    std::atomic<size_t> weak_cnt;

public:
    size_t getStrongCnt() {
        return strong_cnt;
    }

    size_t getWeakCnt() {
        return weak_cnt;
    }

    void increment_strong() {
        strong_cnt.fetch_add(1, std::memory_order_relaxed);
    }

    size_t decrement_strong() {
        return strong_cnt.fetch_sub(1, std::memory_order_acq_rel);
    }

    void increment_weak() {
        weak_cnt.fetch_add(1, std::memory_order_relaxed);
    }

    size_t decrement_weak() {
        return weak_cnt.fetch_sub(1, std::memory_order_acq_rel);
    }

    virtual void destroy_object() = 0;
    virtual ~control_block_base() {}
};

template<typename T, typename Del = DefaultDeleter<T>>
class control_block : public control_block_base {
    T* ptr;
    Del del;
public:
    control_block(T* ptr_) : ptr{ptr_}, del{} {
        increment_strong();
        increment_weak();
    }
    control_block(T* ptr_, Del del_) : ptr{ptr_}, del{del_} {
        increment_strong();
        increment_weak();
    }

    void destroy_object() {
        del(ptr);
    }
};


}