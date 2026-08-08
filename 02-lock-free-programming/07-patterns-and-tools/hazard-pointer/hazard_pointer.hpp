#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <iostream>
#include <set>
#include <thread>

template <size_t S, typename T>
class HazardPointer;

// We expect 1 thread only own 1 HazardPointerOwner object
template <size_t S, typename T>
class HazardPointerOwner{
private:
    size_t _index;
    HazardPointer<S, T>* _parentPtr;

public:
    HazardPointerOwner(size_t index, HazardPointer<S, T>* parentPtr): _index{index}, _parentPtr{parentPtr} {}

    ~HazardPointerOwner() {
        _parentPtr->reset(_index);
    }

    void protect(T* ptr) {
        _parentPtr->protect(_index, ptr);
    }

    void retire(T* ptr) {
        _parentPtr->retire(_index, ptr);
    }
};

template <size_t S, typename T>
class HazardPointer {
private:

    std::array<std::atomic<std::thread::id>, S> _owner;
    // We expect 1 thread only own 1 HazardPointerOwner object, so no atomic
    std::array<std::array<std::atomic<T*>, 2>, S> _pointerProtect;
    //
    std::array<std::atomic<T*>, S * 8> _retireList;

    std::atomic<bool> _doDeletion{false};

    void protect(size_t idx, T* ptr) {
        T* pointerProtect0 = _pointerProtect[idx][0].load(std::memory_order_acquire);
        T* pointerProtect1 = _pointerProtect[idx][1].load(std::memory_order_acquire);
        // It's already protected
        if (pointerProtect0 == ptr || pointerProtect1 == ptr) {
            return;
        }

        // No space to do protect
        if (pointerProtect0 != nullptr && pointerProtect1 != nullptr) {
            return;
        }

        T* expected = nullptr;
        if (_pointerProtect[idx][0].compare_exchange_strong(expected, ptr, std::memory_order_acq_rel)) {
            return;
        }
        
        if (_pointerProtect[idx][1].compare_exchange_strong(expected, ptr, std::memory_order_acq_rel)) {
            return;
        }
    }

    void retire(size_t index, T* ptr) {
        T* pointerProtect0 = _pointerProtect[index][0].load(std::memory_order_acquire);
        T* pointerProtect1 = _pointerProtect[index][1].load(std::memory_order_acquire);

        if (ptr == pointerProtect0) {
            _pointerProtect[index][0].store(nullptr, std::memory_order_release);
        }

        if (ptr == pointerProtect1) {
            _pointerProtect[index][1].store(nullptr, std::memory_order_release);
        }

        bool found = false;
        // Assuming for now, it will be never full because the size is big enough
        while (!found) {
            for (size_t i = 0; i < _retireList.size(); i++) {
                T* expected = nullptr;
                if (_retireList[i].compare_exchange_strong(expected, ptr, std::memory_order_acq_rel)) {
                    found = true;
                    break;
                }
            }
        }

        bool expected = false;
        // Do deletion
        if (!_doDeletion.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) {
            return;
        }

        std::set<T*> ptrSetDeleted;
        std::set<T*> ptrSetChecked;
        for (size_t i = 0; i < _retireList.size(); i++) {
            T* ptr = _retireList[i].load(std::memory_order_acquire);
            // Already did this pointer
            if (ptrSetChecked.find(ptr) != ptrSetChecked.end()) {
                _retireList[i].compare_exchange_strong(ptr, nullptr, std::memory_order_acq_rel);
                continue;
            }

            if (ptrSetDeleted.find(ptr) != ptrSetDeleted.end()) {
                _retireList[i].compare_exchange_strong(ptr, nullptr, std::memory_order_acq_rel);
                continue;
            }

            bool ptrStillProtected = false;
            for (size_t j = 0; j < _pointerProtect.size(); j++) {
                ptrStillProtected |= _pointerProtect[j][0].load(std::memory_order_acquire) == ptr || _pointerProtect[j][1].load(std::memory_order_acquire) == ptr;
                if (ptrStillProtected) {
                    break;
                }
            }

            if (ptrStillProtected) {
                ptrSetChecked.insert(ptr);
                continue;
            }
            
            delete ptr;
            std::cout << "Deleted" << std::endl;
            ptrSetDeleted.insert(ptr);

            T* expectedPtr = ptr;
            _retireList[i].compare_exchange_strong(expectedPtr, nullptr, std::memory_order_acq_rel);
        }

        _doDeletion.store(false, std::memory_order_release);
    }

    void reset(size_t index) {
        _pointerProtect[index][0].store(nullptr, std::memory_order_release);
        _pointerProtect[index][1].store(nullptr, std::memory_order_release);
    }

    friend HazardPointerOwner<S, T>;

public:

    HazardPointer() {
        for (size_t i = 0; i < S; i++) {
            _owner[i] = std::thread::id{};
            _pointerProtect[i][0] = nullptr;
            _pointerProtect[i][1] = nullptr;
        }
    }

    HazardPointer(const HazardPointer&) = delete;
    HazardPointer& operator=(const HazardPointer&) = delete;

    HazardPointerOwner<S, T> make_owner() {
        size_t idx = 0;
        std::thread::id threadId = std::this_thread::get_id();
        for (size_t i = 0; i < _owner.size(); i++) {
            std::thread::id expected{};
            if (_owner[i].compare_exchange_strong(expected, threadId)) {
                idx = i;
                break;
            }
        }

        return HazardPointerOwner<S, T>(idx, this);
    }
};
