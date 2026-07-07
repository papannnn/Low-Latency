#include <condition_variable>
#include <chrono>
#include <iostream>
#include <mutex>
#include <set>
#include <shared_mutex>
#include <thread>

class Barber {
private:
    std::mutex _barberMtx;
    std::mutex _customerMtx;
    std::mutex _setMtx;
    std::shared_mutex _setDoneMtx;
    std::mutex _logMtx;
    std::condition_variable _barberState{};
    std::condition_variable _customerState{};
    std::set<int> _setCust{};
    std::set<int> _doneCust{};

    void log(std::string s) {
        std::lock_guard<std::mutex> lock(_logMtx);
        std::cout << s << std::endl;
    }

    void insertSet(int customerNo) {
        std::lock_guard<std::mutex> lock(_setMtx);
        _setCust.insert(customerNo);
    }

    size_t readSetSize() {
        std::lock_guard<std::mutex> lock(_setMtx);
        return _setCust.size();
    }

    int getCustomerNo() {
        std::lock_guard<std::mutex> lock(_setMtx);
        int customerNo = *_setCust.begin();
        _setCust.erase(customerNo);
        return customerNo;
    }

    void insertDoneCust(int custNo) {
        std::lock_guard<std::shared_mutex> lock(_setDoneMtx);
        _doneCust.insert(custNo);
    }

    bool checkCustDone(int custNo) {
        std::shared_lock<std::shared_mutex> lock(_setDoneMtx);
        return _doneCust.find(custNo) != _doneCust.end();
    }

    void removeCustDone(int custNo) {
        std::lock_guard<std::shared_mutex> lock(_setDoneMtx);
        _doneCust.erase(custNo);
    }

public:
    void exec() {
        std::unique_lock<std::mutex> lock(_barberMtx);

        if (readSetSize() == 0) {
            log("Barber goes to sleep");
            _barberState.wait(lock, [&] () {
                return readSetSize() != 0;
            });
            log("Barber wake up");
        }

        int custNo = getCustomerNo();
        log(std::format("Barber will cut customer {} hair", custNo));

        std::this_thread::sleep_for(std::chrono::seconds(1));

        log(std::format("Barber finished cut customer {} hair", custNo));
        insertDoneCust(custNo);
        _customerState.notify_all();
    }

    void queue(int customerNumber) {
        log(std::format("Customer no: {} start queueing", customerNumber));
        
        insertSet(customerNumber);

        std::unique_lock<std::mutex> lock(_customerMtx);

        _barberState.notify_one();

        _customerState.wait(lock, [&] () {
            return checkCustDone(customerNumber);
        });

        removeCustDone(customerNumber);
        log(std::format("Customer no: {} are finished cutting it's hair", customerNumber));
    }
};