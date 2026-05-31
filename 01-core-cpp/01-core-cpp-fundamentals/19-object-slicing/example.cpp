#include <iostream>
#include <string>

class Base {
public:
    virtual std::string getName() {
        return "Base";
    }
};

class Derived : public Base {
public:
    std::string getName() override {
        return "Derived";
    }
};

int main () {
    Derived derived;
    Base& base = derived;

    std::cout << base.getName() << std::endl;
}