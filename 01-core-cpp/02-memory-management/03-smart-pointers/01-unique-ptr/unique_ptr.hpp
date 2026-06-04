#pragma once

namespace low_latency {

template<typename T, typename Del, bool isEmpty = std::is_empty_v<Del> && !std::is_final_v<Del>>
class compressed_pair;

template<typename T, typename Del>
class compressed_pair<T, Del, false> {
    T* ptr;
    Del deleter;

public:
    compressed_pair(T* ptr_, Del deleter_) : ptr(ptr_), deleter(deleter_) {}
    compressed_pair(compressed_pair&& that) : ptr{that.ptr}, deleter{std::move(that.deleter)} {
        that.ptr = nullptr;
    }
    compressed_pair& operator=(compressed_pair&& that) {
        if (this != &that) {
            ptr = that.ptr;
            deleter = that.deleter;
            that.ptr = nullptr;
        }
        
        return *this;
    }

    T* first() {
        return ptr;
    }

    Del& second() {
        return deleter;
    }
};

template<typename T, typename Del>
class compressed_pair<T, Del, true> : public Del {
    T* ptr;

public:
    explicit compressed_pair(T* ptr_) : ptr(ptr_) {}
    compressed_pair(compressed_pair&& that) : ptr{that.ptr} {
        that.ptr = nullptr;
    }
    compressed_pair& operator=(compressed_pair&& that) {
        if (this != &that) {
            ptr = that.ptr;

            that.ptr = nullptr;
        }

        return *this;
    }

    T* first() {
        return ptr;
    }

    Del& second() {
        return *this;
    }
};

template<typename T>
struct DefaultDeleter {
    void operator()(T* ptr) {
        delete ptr;
    }
};

template<typename T, typename Deleter = DefaultDeleter<T>>
class unique_ptr {
    compressed_pair<T, Deleter> data{};

public:
    explicit unique_ptr(T* ptr_ = nullptr) : data{ptr_} { }

    unique_ptr(T* ptr_, Deleter deleter_) : data{ptr_, deleter_} {}

    unique_ptr(const unique_ptr& that) = delete;
    unique_ptr& operator=(const unique_ptr& that) = delete;

    unique_ptr(unique_ptr&& that) : data{std::move(that.data)} {}

    unique_ptr& operator=(unique_ptr&& that) {
        if (this != &that) {
            data.second()(data.first());
            data = std::move(that.data);
        }
        return *this;
    }

    ~unique_ptr() {
        data.second()(data.first());
    }

    T* operator->() {
        return data.first();
    }

    T& operator*() {
        return *(data.first());
    }

    operator bool() {
        return data.first() != nullptr;
    }
};

}