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
    compressed_pair(T* ptr_) noexcept : ptr(ptr_) {}
    compressed_pair(T* ptr_, Del del_) noexcept : ptr(ptr_), del(std::move(del_)) {}
    compressed_pair(compressed_pair&& that) noexcept : ptr(std::exchange(that.ptr, nullptr)), del(std::move(that.del)) {}
    compressed_pair& operator=(compressed_pair&& that) noexcept {
        ptr = std::exchange(that.ptr, nullptr);
        del = std::move(that.del);
        return *this;
    }

    T* first() noexcept {
        return ptr;
    }

    T* first() const noexcept {
        return ptr;
    }

    Del& second() noexcept {
        return del;
    }

    Del& second() const noexcept {
        return del;
    }
};

template<typename T, typename Del>
class compressed_pair<T, Del, true> : private Del {
    T* ptr;
public:
    compressed_pair() = default;
    compressed_pair(T* ptr_) noexcept : ptr(ptr_) {}
    compressed_pair(T* ptr_, Del del_) noexcept : Del(std::move(del_)), ptr(ptr_) {}
    compressed_pair(compressed_pair&& that) noexcept : Del(std::move(that.second())), ptr(std::exchange(that.ptr, nullptr)) {}
    compressed_pair& operator=(compressed_pair&& that) noexcept {
        second() = std::move(that.second());
        ptr = std::exchange(that.ptr, nullptr);
        return *this;
    }

    T* first() noexcept {
        return ptr;
    }

    T* first() const noexcept {
        return ptr;
    }

    Del& second() noexcept {
        return *this;
    }

    Del& second() const noexcept {
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
    unique_ptr(T* ptr_) noexcept : data{ptr_} {}
    unique_ptr(T* ptr_, Del del_) noexcept : data{ptr_, del_} {}
    
    unique_ptr(const unique_ptr& that) = delete;
    unique_ptr& operator=(const unique_ptr& that) = delete;

    unique_ptr(unique_ptr&& that) noexcept : data(std::move(that.data)) {}
    unique_ptr& operator=(unique_ptr&& that) noexcept {
        if (this != &that) {
            deleteData();
            data = std::move(that.data);
        }
        return *this;
    }

    T* operator->() noexcept {
        return data.first();
    }

    T* operator->() const noexcept {
        return data.first();
    }

    T& operator*() noexcept {
        return *(data.first());
    }

    T& operator*() const noexcept {
        return *(data.first());
    }

    explicit operator bool() noexcept {
        return data.first() != nullptr;
    }

    explicit operator bool() const noexcept {
        return data.first() != nullptr;
    }

    ~unique_ptr() {
        deleteData();
    }
};

}