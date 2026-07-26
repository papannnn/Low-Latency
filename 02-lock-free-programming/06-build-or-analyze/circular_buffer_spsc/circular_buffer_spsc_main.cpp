#include "circular_buffer_spsc.hpp"
#include <iostream>
#include <optional>
#include <thread>

int main () {
    CircularBuffer<size_t> spsc(512);

    std::jthread producer([&] () {
        size_t val = 1;
        while (val < 1000000LL) {
            bool success = spsc.push(val);
            if (success) {
                val++;
            }
        }
    });

    std::jthread consumer([&] () {
        size_t val{};
        while (val < 999999LL) {
            std::optional<size_t> curr = spsc.pop();
            if (curr) {
                val = curr.value();
                std::cout << "Getting value: " << val << std::endl;
            }
        }
    });
}