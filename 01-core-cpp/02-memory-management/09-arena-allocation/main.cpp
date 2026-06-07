#include <iostream>
#include "arena_allocator.hpp"

class Animal {
public:
    std::string name;

    Animal() = default;
    Animal(std::string name_) : name{name_} {}

    ~Animal() {
        std::cout << "Destructor of " << name << " called" << std::endl;
    }
};

int main () {
    low_latency::arena_allocator arena(1024);

    Animal* a = arena.allocate<Animal>();
    a->name = "Andre";
    std::cout << a->name << std::endl;

    Animal* b = arena.allocate<Animal>("Prabowo");
    std::cout << b->name << std::endl;
}