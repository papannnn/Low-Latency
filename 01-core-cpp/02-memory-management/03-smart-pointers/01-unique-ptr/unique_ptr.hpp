#pragma once

namespace low_latency {

template<typename T, typename Del, bool isEmptyAndFinal = std::is_empty_v<Del> && !std::is_final_v<Del>>
class compressed_pair;

template<typename T, typename Del>
class compressed_pair<T, Del, false> {
    T* ptr;
    Del del;

public:
    compressed_pair() = default;
    compressed_pair(T* ptr_) : ptr(ptr_) {}
    compressed_pair(T* ptr_, Del del_) : ptr(ptr_), del(std::move(del_)) {}
    compressed_pair(compressed_pair&& that) : ptr(std::exchange(that.ptr, nullptr)), del(std::move(that.del)) {}
    compressed_pair& operator=(compressed_pair&& that) {
        ptr = std::exchange(that.ptr, nullptr);
        del = std::move(that.del);
        return *this;
    }

    T* first() {
        return ptr;
    }

    Del& second() {
        return del;
    }
};

template<typename T, typename Del>
class compressed_pair<T, Del, true> : private Del {
    T* ptr;
public:
    compressed_pair() = default;
    compressed_pair(T* ptr_) : ptr(ptr_) {}
    compressed_pair(T* ptr_, Del del_) : Del(std::move(del_)), ptr(ptr_) {}
    compressed_pair(compressed_pair&& that) : Del(std::move(that.second())), ptr(std::exchange(that.ptr, nullptr)) {}
    compressed_pair& operator=(compressed_pair&& that) {
        second() = std::move(that.second());
        ptr = std::exchange(that.ptr, nullptr);
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
    void operator() (T* ptr) {
        delete ptr;
    }
};

template<typename T, typename Del = DefaultDeleter<T>>
class unique_ptr {
    compressed_pair<T, Del> data{};
    
    void deleteData() {
        data.second()(data.first());
    }

public:
    unique_ptr(T* ptr_) : data{ptr_} {}
    unique_ptr(T* ptr_, Del del_) : data{ptr_, del_} {}
    
    unique_ptr(const unique_ptr& that) = delete;
    unique_ptr& operator=(const unique_ptr& that) = delete;

    unique_ptr(unique_ptr&& that) : data(std::move(that.data)) {}
    unique_ptr& operator=(unique_ptr&& that) {
        if (this != &that) {
            deleteData();
            data = std::move(that.data);
        }
        return *this;
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

    ~unique_ptr() {
        deleteData();
    }
};

}