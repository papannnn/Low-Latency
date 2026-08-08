#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <iostream>
#include <new>
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

    void unprotect(T* ptr) {
        _parentPtr->unprotect(_index, ptr);
    }

    void retire(T* ptr) {
        _parentPtr->retire(ptr);
    }
};

template <typename T>
struct AtomicValuePadded {
    alignas(std::hardware_destructive_interference_size) std::atomic<T> value;
};

template <size_t S, typename T>
class HazardPointer {
private:

    std::array<AtomicValuePadded<std::thread::id>, S> _owner;
    // We expect 1 thread only own 1 HazardPointerOwner object, so no atomic
    std::array<std::array<AtomicValuePadded<T*>, 2>, S> _pointerProtect;
    //
    std::array<AtomicValuePadded<T*>, S * 8> _retireList;

    alignas(std::hardware_destructive_interference_size) std::atomic<bool> _doDeletion{false};

    void protect(size_t idx, T* ptr) {
        T* pointerProtect0 = _pointerProtect[idx][0].value.load(std::memory_order_acquire);
        T* pointerProtect1 = _pointerProtect[idx][1].value.load(std::memory_order_acquire);
        // It's already protected
        if (pointerProtect0 == ptr || pointerProtect1 == ptr) {
            return;
        }

        // No space to do protect
        if (pointerProtect0 != nullptr && pointerProtect1 != nullptr) {
            return;
        }

        T* expected = nullptr;
        if (_pointerProtect[idx][0].value.compare_exchange_strong(expected, ptr, std::memory_order_acq_rel)) {
            return;
        }
        
        if (_pointerProtect[idx][1].value.compare_exchange_strong(expected, ptr, std::memory_order_acq_rel)) {
            return;
        }
    }

    void unprotect(size_t index, T* ptr) {
        T* pointerProtect0 = _pointerProtect[index][0].value.load(std::memory_order_acquire);
        T* pointerProtect1 = _pointerProtect[index][1].value.load(std::memory_order_acquire);

        if (ptr == pointerProtect0) {
            _pointerProtect[index][0].value.store(nullptr, std::memory_order_release);
        }

        if (ptr == pointerProtect1) {
            _pointerProtect[index][1].value.store(nullptr, std::memory_order_release);
        }
    }

    void retire(T* ptr) {
        bool found = false;
        // Assuming for now, it will be never full because the size is big enough
        while (!found) {
            for (size_t i = 0; i < _retireList.size(); i++) {
                T* expected = nullptr;
                if (_retireList[i].value.compare_exchange_strong(expected, ptr, std::memory_order_acq_rel)) {
                    found = true;
                    break;
                }
            }
        }

        deleteRetire();
    }

    void deleteRetire() {
        bool expected = false;
        // Do deletion
        if (!_doDeletion.compare_exchange_strong(expected, true, std::memory_order_acq_rel)) {
            return;
        }

        std::set<T*> ptrSetDeleted;
        std::set<T*> ptrSetChecked;
        for (size_t i = 0; i < _retireList.size(); i++) {
            T* ptr = _retireList[i].value.load(std::memory_order_acquire);
            // Already did this pointer
            if (ptrSetChecked.find(ptr) != ptrSetChecked.end()) {
                _retireList[i].value.compare_exchange_strong(ptr, nullptr, std::memory_order_acq_rel);
                continue;
            }

            // This pointer already deleted
            if (ptrSetDeleted.find(ptr) != ptrSetDeleted.end()) {
                _retireList[i].value.compare_exchange_strong(ptr, nullptr, std::memory_order_acq_rel);
                continue;
            }

            bool ptrStillProtected = false;
            for (size_t j = 0; j < _pointerProtect.size(); j++) {
                ptrStillProtected |= _pointerProtect[j][0].value.load(std::memory_order_acquire) == ptr || _pointerProtect[j][1].value.load(std::memory_order_acquire) == ptr;
                if (ptrStillProtected) {
                    break;
                }
            }

            if (ptrStillProtected) {
                ptrSetChecked.insert(ptr);
                continue;
            }
            
            delete ptr;
            ptrSetDeleted.insert(ptr);

            T* expectedPtr = ptr;
            _retireList[i].value.compare_exchange_strong(expectedPtr, nullptr, std::memory_order_acq_rel);
        }

        _doDeletion.store(false, std::memory_order_release);
    }

    void reset(size_t index) {
        _pointerProtect[index][0].value.store(nullptr, std::memory_order_relaxed);
        _pointerProtect[index][1].value.store(nullptr, std::memory_order_relaxed);
        _owner[index].value.store(std::thread::id{}, std::memory_order_release);
    }

    friend HazardPointerOwner<S, T>;

public:

    HazardPointer() {
        for (size_t i = 0; i < S; i++) {
            _owner[i].value.store(std::thread::id{}, std::memory_order_relaxed);
            _pointerProtect[i][0].value.store(nullptr, std::memory_order_relaxed);
            _pointerProtect[i][1].value.store(nullptr, std::memory_order_relaxed);;
        }
    }

    // I assume all owner got killed already, so I just need to handle the retire list
    ~HazardPointer() {
        deleteRetire();
    }

    HazardPointer(const HazardPointer&) = delete;
    HazardPointer& operator=(const HazardPointer&) = delete;

    HazardPointerOwner<S, T> make_owner() {
        // I purposely didn't make HazardPointer full logic
        size_t idx = 0;
        std::thread::id threadId = std::this_thread::get_id();
        for (size_t i = 0; i < _owner.size(); i++) {
            std::thread::id expected{};
            if (_owner[i].value.compare_exchange_strong(expected, threadId)) {
                idx = i;
                break;
            }
        }

        return HazardPointerOwner<S, T>(idx, this);
    }
};
