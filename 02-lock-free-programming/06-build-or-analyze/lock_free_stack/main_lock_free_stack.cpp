#include "lock_free_stack.hpp"

#include <iostream>
#include <string>

int main () {
    ConcurrentStack<std::string> stk;

    stk.push("Hello");

    std::optional<std::string> d = stk.pop();
    if (d) {
        std::cout << d.value() << std::endl;
    }
    
    stk.push("World");

    d = stk.pop();
    if (d) {
        std::cout << d.value() << std::endl;
    }
    // stk.push("World");

    // std::cout << stk.top() << std::endl;
}