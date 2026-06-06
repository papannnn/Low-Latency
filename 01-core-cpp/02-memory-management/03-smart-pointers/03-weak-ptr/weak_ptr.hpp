#pragma once

#include "control_block.hpp"

namespace low_latency {

template<typename T>
class shared_ptr;

template<typename T>
class weak_ptr {
    friend class shared_ptr<T>;

    T* ptr = nullptr;
    control_block_base* ctrl = nullptr;

    void decrement() {
        if (ctrl == nullptr) {
            return;
        }

        if (ctrl->decrement_weak() == 1) {
            delete ctrl;
        }
    }

public:
    weak_ptr() = default;

    weak_ptr(const shared_ptr<T>& that) noexcept : ptr(that.get()), ctrl(that.ctrl) {
        if (ctrl != nullptr) {
            ctrl->increment_weak();
        }
    }

    weak_ptr& operator=(const shared_ptr<T>& that) noexcept {
        ptr = that.ptr;
        ctrl = that.ctrl;

        if (ctrl != nullptr) {
            ctrl->increment_weak();
        }
    }

    weak_ptr(const weak_ptr& that) noexcept : ptr(that.ptr), ctrl(that.ctrl)  {
        if (ctrl != nullptr) {
            ctrl->increment_weak();
        }
    }

    weak_ptr& operator=(const weak_ptr& that) noexcept {
        if (this != &that) {
            return *this;
        }

        decrement();

        ptr = that.ptr;
        ctrl = that.ctrl;
        if (ctrl != nullptr) {
            ctrl->increment_weak();
        }
        return *this;
    }

    weak_ptr(weak_ptr&& that) noexcept : ptr(std::exchange(that.ptr, nullptr)), ctrl(std::exchange(that.ctrl, nullptr)) {}
    weak_ptr& operator=(weak_ptr&& that) {
        if (this != &that) {
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

    void swap(weak_ptr& that) noexcept {
        std::swap(ptr, that.ptr);
        std::swap(ctrl, that.ctrl);
    } 

    size_t use_count() noexcept {
        if (ctrl != nullptr) {
            return ctrl->getWeakCnt();
        }
        return 0;
    }

    size_t use_count() const noexcept {
        if (ctrl != nullptr) {
            return ctrl->getWeakCnt();
        }
        return 0;
    }

    bool expired() noexcept {
        if (ctrl != nullptr) {
            return ctrl->getStrongCnt() == 0;
        }
        return true;
    }

    bool expired() const noexcept {
        if (ctrl != nullptr) {
            return ctrl->getStrongCnt() == 0;
        }
        return true;
    }

    shared_ptr<T> lock() noexcept {
        if (expired()) {
            return {};
        }

        return shared_ptr<T>(*this);
    }
};

}