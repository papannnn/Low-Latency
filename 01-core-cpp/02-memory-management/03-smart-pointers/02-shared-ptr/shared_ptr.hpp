#pragma once

#include <atomic>
#include <memory>

namespace low_latency {

class ControlBlock {
    std::atomic<size_t> strong_reference_cnt;

public:
    ControlBlock() {
        strong_reference_cnt.store(1, std::memory_order_relaxed);
    }

    void increment_strong() {
        strong_reference_cnt.fetch_add(1, std::memory_order_relaxed);
    }

    size_t decrement_strong() {
        return strong_reference_cnt.fetch_sub(1, std::memory_order_acq_rel);
    }
};

template<typename T>
class shared_ptr {
    T* ptr;
    ControlBlock *block;

    void cleanup() {
        delete ptr;
        delete block;
    }

public:
    shared_ptr(T* ptr_) : ptr(ptr_), block(new ControlBlock) {}
    shared_ptr(const shared_ptr& that) : ptr(that.ptr), block(that.block) {
        if (block != nullptr) {
            block->increment_strong();
        }
    }
    shared_ptr& operator=(const shared_ptr& that) {
        if (this == &that) {
            return *this;
        }

        if (block != nullptr && block->decrement_strong() == 1) {
            cleanup();
        }

        ptr = that.ptr;
        block = that.block;
        if (block != nullptr) {
            block->increment_strong();
        }

        return *this;
    }

    shared_ptr(shared_ptr&& that) : ptr(std::exchange(that.ptr, nullptr)), block(std::exchange(that.block, nullptr)) {}
    shared_ptr& operator=(shared_ptr&& that) {
        if (this == &that) {
            return *this;
        }

        if (block != nullptr && block->decrement_strong() == 1) {
            cleanup();
        }

        ptr = std::exchange(that.ptr, nullptr);
        block = std::exchange(that.block, nullptr);
        return *this;
    }

    T* operator->() const {
        return ptr;
    }

    T& operator*() const {
        return *ptr;
    }

    operator bool() const {
        return ptr != nullptr;
    }

    ~shared_ptr() {
        if (block != nullptr && block->decrement_strong() == 1) {
            cleanup();
        }
    }
};

};