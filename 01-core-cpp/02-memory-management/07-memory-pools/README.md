# Memory Pools

## Pool allocation

Pool allocation is basically allocating a large single memory to be used for smaller object in the future.

## Fixed-size blocks

Usually we already know what kind of the object that we want to create / destroy, so we know the size of the object already.

## Allocation and deallocation speed

Compared to malloc / new, the speed of using memory pool is faster since it's not triggering any syscall since we already did that for the large chunk of the memory pool.

## Tradeoff

It will add complexity to the code, this is also not a silver bullet since you need to know what kind of object to use this memory pool.

## Source

https://www.geeksforgeeks.org/system-design/what-is-a-memory-pool/