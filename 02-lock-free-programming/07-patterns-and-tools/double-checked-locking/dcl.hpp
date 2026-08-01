#include <atomic>
#include <iostream>
#include <mutex>

class Singleton {
private:
    inline static std::atomic<Singleton*> INSTANCE;
    inline static std::mutex _mtx;

    Singleton() {
        std::cout << "Singleton created" << std::endl;
    }

public:
    static Singleton* getInstance() {
        Singleton* result = Singleton::INSTANCE.load(std::memory_order_acquire);
        if (result == nullptr) {
            std::lock_guard<std::mutex> lck(Singleton::_mtx);
            result = Singleton::INSTANCE.load(std::memory_order_relaxed);
            if (result == nullptr) {
                result = new Singleton();
                Singleton::INSTANCE.store(result, std::memory_order_release);
            }
        }
        return result;
    }

    void doSomething() {
        std::cout << "something" << std::endl;
    }
};