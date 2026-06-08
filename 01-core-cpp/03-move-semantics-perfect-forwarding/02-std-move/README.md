# `std::move`

## What `std::move` 

`std::move` is actually not moving anything. It's basically a compile time casting to rvalue.

## Moved-from object

Moved from object is an object that has been moved to another one.

## When to use `std::move`

When you don't want to do copy and copying is more slower than moving

## Source

https://jaredmil.medium.com/move-semantics-pt-3-std-move-explained-simply-337aa3061d0d

https://andreasfertig.com/blog/2021/04/can-i-touch-a-moved-from-object/