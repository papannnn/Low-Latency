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
        // It's already protected
        if (_pointerProtect[idx][0] == ptr || _pointerProtect[idx][1] == ptr) {
            return;
        }

        // No space to do protect
        if (_pointerProtect[idx][0] != nullptr && _pointerProtect[idx][1] != nullptr) {
            return;
        }

        if (_pointerProtect[idx][0] == nullptr) {
            _pointerProtect[idx][0] = ptr;
        }

        if (_pointerProtect[idx][1] == nullptr) {
            _pointerProtect[idx][1] = ptr;
        }
    }

    void retire(size_t index, void* ptr) {
        bool found = _pointerProtect[index][0] == ptr || _pointerProtect[index][1] == ptr;
        if (_pointerProtect[index][0].load(std::memory_order_acquire) == ptr) {
            _pointerProtect[index][0].store(nullptr, std::memory_order_release);
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
