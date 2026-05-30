# Implement a Custom `std::function`

## Problem

Build a type-erased callable wrapper that can store lambdas, function pointers, and functors.

## Requirements

- Support type erasure.
- Store multiple callable types.
- Implement small buffer optimization to avoid heap allocation for small callables.
