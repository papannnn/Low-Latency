#include <iostream>

// Vtable type definition
struct Animal_VTable {
    // A function pointer that takes a void * (this pointer)
    void (*speak)(void *self);
};

// Equivalent to: class Animal { ... }
struct Animal {
    const Animal_VTable* vptr; // Hidden pointer
    const char* name;
};

// Equivalent to: class Dog : public Animal { ... }
struct Dog {
    Animal base;
    int breed_id;
};

void Animal_speak(void *self) {
    std::cout << "Animal speak" << std::endl;
}

void Dog_speak(void *self) {
    std::cout << "Woof" << std::endl;
}

static const Animal_VTable Animal_VT = {
    &Animal_speak
};

static const Animal_VTable Dog_VT = {
    &Dog_speak
};

void Animal_Construct(Animal *self) {
    self->vptr = &Animal_VT;
    self->name = "Unamed";
}

void Dog_Construct(Dog *self) {
    Animal_Construct(&self->base);

    self->base.vptr = &Dog_VT;
    self->breed_id = 101;
}

int main () {
    Dog dog;
    Dog_Construct(&dog);

    Animal* animal = (Animal*) &dog;
    animal->vptr->speak(animal);
}