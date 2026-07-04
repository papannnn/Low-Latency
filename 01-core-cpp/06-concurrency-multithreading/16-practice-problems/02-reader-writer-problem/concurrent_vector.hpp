#include <iostream>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <string>

template <typename T>
class ConcurrentVector {
private:
    std::vector<T> _vec{};
    std::shared_mutex _mtx{};
public:
    void print(const std::string& threadName) {
        std::shared_lock<std::shared_mutex> lock(_mtx);

        std::cout << "Printing value from: " << threadName << " size is " << _vec.size() << std::endl;
    }

    void push(T ele) {
        std::lock_guard<std::shared_mutex> lock(_mtx);

        std::cout << "Pushing value " << ele << " to vector" << std::endl;
        _vec.push_back(std::move(ele));
    }
};