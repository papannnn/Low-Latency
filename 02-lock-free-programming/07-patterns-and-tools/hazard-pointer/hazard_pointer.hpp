#pragma once

#include <array>
#include <atomic>
#include <cstddef>

template <size_t S>
class HazardPointer;

// We expect 1 thread only own 1 HazardPointerOwner object
template <size_t S>
class HazardPointerOwner{
private:
    size_t _index;
    HazardPointer<S>* _parentPtr;

public:
    void protect(void* ptr) {
        _parentPtr->protect(_index, ptr);
    }

    void retire(void* ptr) {
        _parentPtr->retire(_index, ptr);
    }
};

template <size_t S>
class HazardPointer {
private:

    std::array<size_t, S> _owner;
    // We expect 1 thread only own 1 HazardPointerOwner object, so no atomic
    std::array<std::array<std::atomic<void*>, 2>, S> _pointerProtect;
    //
    std::array<std::atomic<void*>, S * 4> _retireList;

    void protect(size_t idx, void* ptr) {
        void* pointerProtect0 = _pointerProtect[idx][0].load(std::memory_order_acquire);
        void* pointerProtect1 = _pointerProtect[idx][1].load(std::memory_order_acquire);
        // It's already protected
        if (pointerProtect0 == ptr || pointerProtect1 == ptr) {
            return;
        }

        // No space to do protect
        if (pointerProtect0 != nullptr && pointerProtect1 != nullptr) {
            return;
        }

        void* expected = nullptr;
        if (_pointerProtect[idx][0].compare_exchange_strong(expected, ptr)) {
            return;
        }
        
        if (_pointerProtect[idx][1].compare_exchange_strong(expected, ptr)) {
            return;
        }
    }

    void retire(size_t index, void* ptr) {
        void* pointerProtect0 = _pointerProtect[index][0].load(std::memory_order_acquire);
        void* pointerProtect1 = _pointerProtect[index][1].load(std::memory_order_acquire);

        bool found = pointerProtect0 == ptr || pointerProtect1 == ptr;
        std::atomic<void*> dummy;
        void* expected = ptr;
        dummy.compare_exchange_strong(expected, ptr);
        if (_pointerProtect[index][0].compare_exchange_strong(expected, ptr)) {
            
        }

        if (_pointerProtect[index][1].load(std::memory_order_acquire) == ptr) {
            _pointerProtect[index][1].store(nullptr, std::memory_order_release);
        }

        if (!found) {
            return;
        }


    }

    friend HazardPointerOwner<S>;

public:

    HazardPointer() {
        for (size_t i = 0; i < S; i++) {
            _owner[i] = 0;
            _pointerProtect[i][0] = nullptr;
            _pointerProtect[i][1] = nullptr;
        }
    }

    HazardPointer(const HazardPointer&) = delete;
    HazardPointer& operator=(const HazardPointer&) = delete;

};
