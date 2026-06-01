# Polymorphism Cost Analysis

## Pointer Chasing

As explained previously on [Virtual Table Internal](../16-vtable-internals/README.md)

Calling a virtual function need to do pointer chasing, compared to class that doesn't use virtual function, it knows the location since the location is static.

## Branch Prediction

It will affect branch prediction in CPU Pipeline, especially when the object access very random. Since it determined at runtime, CPU doesn't know which function address it should fetch.

## Alternative to runtime polymorphism

- Template
- std::variant & std::visit
- std::function

## Source

https://www.youtube.com/watch?v=3ux5YEj1c54

https://softwareengineering.stackexchange.com/questions/191637/in-c-why-and-how-are-virtual-functions-slower

