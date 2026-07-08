#include <condition_variable>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <format>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

class Agent {
private:
    std::mutex _logMtx;
    std::mutex _agentMtx;
    std::mutex _smokerMtx;
    std::mutex _tableMtx;
    std::condition_variable _agentCv;
    std::condition_variable _smokerCv;
    
    int _table = EMPTY;

    const int choices[3] = {TOBACCO | PAPER, TOBACCO | MATCHES, PAPER | MATCHES};
    const std::string choicesName[3] = {"TOBACCO | PAPER", "TOBACCO | MATCHES", "PAPER | MATCHES"};

    void log(std::string s) {
        std::lock_guard<std::mutex> lock(_logMtx);
        std::cout << s << std::endl;
    }

    void updateTable(int item) {
        std::lock_guard<std::mutex> lock(_tableMtx);
        _table = item;
    }

public:
    static const int EMPTY = 0;
    static const int TOBACCO = 1;
    static const int PAPER = 2;
    static const int MATCHES = 4;
    static const int ALL = TOBACCO | PAPER | MATCHES;

    void exec () {
        std::unique_lock<std::mutex> lock(_agentMtx);

        if (_table != EMPTY) {
            _agentCv.wait(lock, [&] () {
                return _table == EMPTY;
            });
        }

        srand(time(0));
        int idxRand = rand() % 3;
        log(std::format("Agent provides {}", choicesName[idxRand]));

        updateTable(choices[idxRand]);
        
        _smokerCv.notify_all();
    }

    void smoke(int smokerNo, int item) {
        std::unique_lock<std::mutex> lock(_smokerMtx);

        if (_table == 0 || (item | _table) != ALL) {
            _smokerCv.wait(lock, [&] () {
                return _table != 0 && (item | _table) == ALL;
            });
        }

        log(std::format("Smoker no {} start smoking", smokerNo));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        log(std::format("Smoker no {} finished smoking", smokerNo));

        updateTable(EMPTY);
        
        _agentCv.notify_one();
    }
};