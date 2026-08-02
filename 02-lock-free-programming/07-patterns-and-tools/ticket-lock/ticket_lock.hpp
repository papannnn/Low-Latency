#include <atomic>

class TicketLock {
private:
    std::atomic<size_t> _line{};
    std::atomic<size_t> _serving{};
public:
    void lock() {
        size_t line = _line.fetch_add(1, std::memory_order_acquire);

        while (line != _serving.load(std::memory_order_acquire));
    }

    void unlock() {
        size_t serving = _serving.load(std::memory_order_acquire);

        _serving.store(serving + 1, std::memory_order_release);
    }
};