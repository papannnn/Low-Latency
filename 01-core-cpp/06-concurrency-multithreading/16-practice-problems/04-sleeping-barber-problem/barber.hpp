#include <atomic>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <mutex>
#include <set>
#include <thread>
#include <vector>


class Barber {
private:
    std::mutex _barberMtx;
    std::mutex _customerMtx;
    std::mutex _logMtx;
    std::condition_variable _barberState{};
    std::condition_variable _customerState{};
    std::condition_variable _customerCuttinghairState{};
    std::atomic<int> _currCust{-1};
    std::set<int> _setCust{};

    void log(std::string s) {
        std::lock_guard<std::mutex> lock(_logMtx);
        std::cout << s << std::endl;
    }

public:
    void exec() {
        std::unique_lock<std::mutex> lock(_barberMtx);
        int custNo = _currCust.load(std::memory_order_acquire);
        if (custNo == -1) {
            _barberState.wait(lock, [&] () {
                bool valid = _setCust.size() != 0;
                if (!valid) {
                    log("No customer, barber go sleep");
                } else {
                    log("Just checking");
                }
                return valid && _currCust.load(std::memory_order_acquire) != -1;
            });
            log("Barber wakes up");
        }
        
        custNo = _currCust.load(std::memory_order_acquire);
        log(std::format("Barber cutting hair customer no: {}", custNo));
        std::this_thread::sleep_for(std::chrono::seconds(2));
        log(std::format("Barber finished cutting hair customer no: {}", custNo));
        
        _currCust.store(-1, std::memory_order_release);
        _customerState.notify_all();
        _customerCuttinghairState.notify_one();
    }

    void queue(int customerNumber) {
        log(std::format("Customer no: {} start queueing", customerNumber));
        _setCust.insert(customerNumber);

        _barberState.notify_one();

        std::unique_lock<std::mutex> lock(_customerMtx);

        log(std::format("Customer no: {} acquire the lock", customerNumber));

        _customerState.wait(lock, [&] () {
            int expected = -1;
            return _currCust.compare_exchange_weak(
                expected, 
                customerNumber, 
                std::memory_order_acq_rel, 
                std::memory_order_acquire
            );
        });
        
        log(std::format("Customer no: {} be ready to cut hair", customerNumber));

        _customerCuttinghairState.wait(lock, [&] () {
            log(std::format("This {} ", _currCust.load(std::memory_order_acquire)));
            return _currCust.load(std::memory_order_acquire) != customerNumber;
        });

        _setCust.erase(customerNumber);

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
};