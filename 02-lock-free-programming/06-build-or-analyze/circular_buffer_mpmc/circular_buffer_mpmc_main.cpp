#include <atomic>
#include "circular_buffer_mpmc.hpp"
#include <iostream>
#include <thread>
#include <vector>


int main () {
    mpmc_queue<unsigned> q{32};

    constexpr unsigned N{1000000};
    constexpr unsigned P{2};
    std::atomic<uint64_t> prod_sum{0};
    std::atomic<uint64_t> cons_sum{0};

    auto producer = [&] () {
        for (unsigned x = 0; x < N; x++) {
            while (!q.push(x));
            prod_sum += x;
        }
    };

    std::vector<std::jthread> producers;
    producers.resize(P);

    for (auto &p : producers) {
        p = std::jthread(producer);
    }

    auto consumer = [&] () {
        unsigned v{0};
        for (unsigned x = 0; x < N; x++) {
            while (!q.pop(v));
            cons_sum += v;
        }
    };

    std::vector<std::jthread> consumers;
    consumers.resize(P);

    for (auto &c : consumers) {
        c = std::jthread(consumer);
    }

    std::cout << (cons_sum && cons_sum == prod_sum ? "OK" : "ERROR") << " " << cons_sum << std::endl;
}