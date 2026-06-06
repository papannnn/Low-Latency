#pragma once

#include "control_block.hpp"
#include <iostream>

namespace low_latency {

template<typename T>
class shared_ptr {
    T* ptr = nullptr;
    control_block_base* ctrl = nullptr;

    void decrement() noexcept {
        if (ctrl == nullptr) {
            return;
        }

        if (ctrl->decrement_strong() == 1) {
            ctrl->destroy_object();

            if (ctrl->decrement_weak() == 1) {
                delete ctrl;
            }
        }
    }

public:
    shared_ptr() noexcept : ptr(nullptr), ctrl(nullptr) {}
    shared_ptr(T* ptr_) noexcept : ptr(ptr_) {
        if (ptr == nullptr) {
            return;
        }

        ctrl = new control_block(ptr_);
    }

    template<typename Del>
    shared_ptr(T* ptr_, Del del_) noexcept : ptr(ptr_) {
        if (ptr == nullptr) {
            return;
        }

        ctrl = new control_block(ptr_, del_);
    }

    shared_ptr(const shared_ptr& that) noexcept : ptr(that.ptr), ctrl(that.ctrl) {
        if (ctrl != nullptr) {
            ctrl->increment_strong();
        }
    }

    shared_ptr& operator=(const shared_ptr& that) noexcept {
        if (this == &that) {
            return *this;
        }

        decrement();

        ptr = that.ptr;
        ctrl = that.ctrl;

        if (ctrl != nullptr) {
            ctrl->increment_strong();
        }
        return *this;
    }

    shared_ptr(shared_ptr&& that) noexcept : ptr(std::exchange(that.ptr, nullptr)), ctrl(std::exchange(that.ctrl, nullptr)) {}
    shared_ptr& operator=(shared_ptr&& that) {
        if (this == &that) {
            return *this;
        }

        decrement();

        ptr = std::exchange(that.ptr, nullptr);
        ctrl = std::exchange(that.ctrl, nullptr);

        return *this;
    }

    void reset() noexcept {
        decrement();
        ptr = nullptr;
        ctrl = nullptr;
    }

    void reset(T* ptr_) {
        decrement();
        ptr = ptr_;
        if (ptr != nullptr) {
            ctrl = new control_block(ptr_);
        } else {
            ctrl = nullptr;
        }
    }

    void swap(shared_ptr& that) noexcept {
        std::swap(ptr, that.ptr);
        std::swap(ctrl, that.ctrl);
    }

    T* get() noexcept {
        return ptr;
    }

    T* get() const noexcept {
        return ptr;
    }

    T* operator->() noexcept {
        return ptr;
    }

    T* operator->() const noexcept {
        return ptr;
    }

    T& operator*() noexcept {
        return *ptr;
    }

    T& operator*() const noexcept {
        return *ptr;
    }

    explicit operator bool() noexcept {
        return ptr != nullptr;
    }

    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }

    size_t use_count() noexcept {
        if (ctrl == nullptr) {
            return 0;
        }

        return ctrl->getStrongCnt();
    }

    size_t use_count() const noexcept {
        if (ctrl == nullptr) {
            return 0;
        }

        return ctrl->getStrongCnt();
    }

    ~shared_ptr() {
        decrement();
    }
};

}