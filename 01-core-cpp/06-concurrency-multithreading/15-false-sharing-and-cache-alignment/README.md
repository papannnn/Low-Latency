# False Sharing and Cache Alignment

## False Sharing

False Sharing is a phenomena that makes your code runs slower when doing multithreading.

It occurs because CPU L1 and L2 cache actually didn't fetch just a single bytes to the RAM, it fetch of the size of cache lines.

For example, Intel CPU's cache lines is 64 bytes. That means if the data not yet cached, CPU will fetch whole 64 bytes in one go.

## Illustration

Assuming there's 2 thread, each of them are owned by different CPU, both of them are holding the same cache line.

```
           Cache Line (64 bytes)
+-------------------------------------------------------------+
| counterA | counterB |        unused bytes...                |
| 4 bytes  | 4 bytes  |                                       |
+-------------------------------------------------------------+
       ^         ^
       |         |
   CPU 0      CPU 1
```

```
                    BEFORE

            CPU 0                     CPU 1
        +------------+            +------------+
        | L1 Cache   |            | L1 Cache   |
        |------------|            |------------|
        |A=100 B=200 | (Shared)   |A=100 B=200 | (Shared)
        +------------+            +------------+
                \                    /
                 \                  /
                  +----------------+
                  | Main Memory    |
                  |A=100  B=200    |
                  +----------------+
```

Then, CPU 0 updates var `A`

```
counterA++
```

Because CPU 1 also cares about the cache line, CPU 0 need to notify CPU 1 to invalidate it's current cache line

```
            CPU 0                     CPU 1
        +------------+            +------------+
        | L1 Cache   |            | L1 Cache   |
        |------------|            |------------|
        |A=101 B=200 | (Modified) |A=100 B=200 |
        +------------+            +------------+
                |                        ^
                |                        |
                +---- Invalidate --------+
                     entire cache line
```

After got notified, now CPU 1 don't cache the cache line anymore

```
            CPU 0                     CPU 1
        +------------+            +------------+
        | L1 Cache   |            | L1 Cache   |
        |------------|            |------------|
        |A=101 B=200 | (Modified) | INVALID    |
        +------------+            +------------+
```

Assuming CPU 1 want to do 

```
counterB++
```

That means CPU 1 need to fetch again from RAM, then make the CPU 0 delete it's cache, and so on. Fetching from RAM is much much slower than fetching from L1 cache, we need to avoid it.

## Solution

Solution to this is to make an alignment on the variable so the address will not sharing the same cache line.

```c++
alignas(64) int g_counter1 = 0;
alignas(64) int g_counter2 = 0;
```

Now, the cache line looks like this

```
                  Cache Line #0 (64 bytes)

+---------------------------------------------------------------+
| g_counter1 | padding | padding | padding | padding | padding  |
|   4 bytes  |                 60 bytes                      |
+---------------------------------------------------------------+

                  Cache Line #1 (64 bytes)

+---------------------------------------------------------------+
| g_counter2 | padding | padding | padding | padding | padding  |
|   4 bytes  |                 60 bytes                      |
+---------------------------------------------------------------+
```

## Source

https://www.aussieai.com/blog/false-sharing
