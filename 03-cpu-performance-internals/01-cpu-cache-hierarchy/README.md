# CPU Cache Hierarchy

## L1, L2, and L3 caches

To improve performance, When CPU want to access memory, it doesn't check directly the memory. 

It will check the CPU cache first.

```
                      /\
                     /  \
                    / REG\        ~0.3 ns     < 1 KB
                   /------\
                  /   L1   \      ~1 ns       32–64 KB  (per core, split I/D)
                 /----------\
                /     L2     \    ~3–5 ns     256 KB–2 MB (per core)
               /--------------\
              /       L3       \  ~10–20 ns   8–64 MB (shared)
             /------------------\
            /    DRAM (memory)   \  ~60–100 ns  GBs
           /----------------------\

   ▲ faster, smaller
   ▼ slower, larger
```

L1 cache is more faster than L2, L2 cache is more faster than L3, and L3 cache is more faster than accessing memory.

But L1 cache is smaller than L2, L2 cache is smaller than L3. There's a tradeoff for each.

When you fetch something from memory, your computer will try to cache that into the cache, when you want to fetch the same thing, CPU doesn't need to go that far to the memory again, it can just go fetch it from cache.

It's like you go grocery shopping far away from town, then all of the grocery you put it inside the fridge, next time you want to get the food, you can just fetch it from the fridge.

## Source

https://youtu.be/7yrK_9PderQ?si=y6kp9xHu-Hct_R1E