#include "lock_free_stack.hpp"

#include <iostream>
#include <string>

int main () {
    ConcurrentStack<std::string> stk;

    stk.push("Hello");

    std::cout << stk.top() << std::endl;

    stk.push("World");

    std::cout << stk.top() << std::endl;
}