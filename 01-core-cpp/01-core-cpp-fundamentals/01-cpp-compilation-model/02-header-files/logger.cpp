#include <iostream>
#include "logger.hpp"

void InitLog() {
    Log("Initializing Log"); // Compilation error, no Log function got declared
}

void Log(const char* message) {
    std::cout << message << std::endl;
}