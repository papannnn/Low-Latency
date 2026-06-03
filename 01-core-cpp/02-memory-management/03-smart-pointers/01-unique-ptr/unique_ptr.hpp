#pragma once

namespace low_latency {

template<typename T>
class unique_ptr {
private:
    T* ptr;
public:
    explicit unique_ptr(T* ptr_);

    // Copy Constructor / Operator
    unique_ptr(const unique_ptr& u_ptr) = delete;
    unique_ptr& operator=(const unique_ptr& u_ptr) = delete;
    
    // Move Constructor
    unique_ptr(unique_ptr&& u_ptr);

    // Move Operator
    unique_ptr& operator=(unique_ptr&& u_ptr);
    // Destructor
    ~unique_ptr();

    T* operator->();

    T& operator*();

    operator bool() const;
};

template<typename T>
unique_ptr<T>::unique_ptr(T* ptr_) : ptr(ptr_) { }

template<typename T>
unique_ptr<T>::unique_ptr(unique_ptr&& u_ptr) : ptr(u_ptr.ptr) {
    u_ptr.ptr = nullptr;
}

template<typename T>
unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr&& u_ptr) {
    if (this != &u_ptr) {
        delete ptr;
        ptr = u_ptr.ptr;
        u_ptr.ptr = nullptr;
    }
    return *this;
}

template<typename T>
unique_ptr<T>::~unique_ptr() {
    delete ptr;
}

template<typename T>
T* unique_ptr<T>::operator->() {
    return ptr;
}

template<typename T>
T& unique_ptr<T>::operator*() {
    return *ptr;
}

template<typename T>
unique_ptr<T>::operator bool() const {
    return ptr != nullptr;
}

}