#include <atomic>

class SpinLock {
private:
    std::atomic_flag _acquiredFlag{};

public:
    SpinLock() {}
    SpinLock(const SpinLock& spinLock) = delete;
    SpinLock& operator=(const SpinLock& spinLock) = delete;

    void lock() {
        while (_acquiredFlag.test_and_set(std::memory_order_acq_rel)) {
            size_t cnt = 0;
            while (_acquiredFlag.test(std::memory_order_acquire)) {
                if (cnt < 10) {
                    cnt++;
                }
                size_t duration = 100;
                
                // Backoff
                for (size_t i = 0 ; i < duration * cnt; i++) {}
            }
        }
    }

    void unlock() {
        _acquiredFlag.clear(std::memory_order_release);
    }
};