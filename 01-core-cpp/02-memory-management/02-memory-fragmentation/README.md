# Memory Fragmentation

## Internal Fragmentation

Internal Fragmentation is when memory is given to you is larger than you ask.

Assuming you ask 10 bytes of memory.

But you got 16 instead.

```
16-byte allocated block

+----+----+----+----+
| U  | U  | U  | W  |
+----+----+----+----+
 4B   4B   2B   6B

U = Used by program
W = Wasted space inside allocated block
```

## External Fragmentation

External Fragmentation is when you have enough memory to give, but the memory can't be given because those memory are not contiguous.

Assuming you want to get 32 bytes.
```
Address →

0         16        32        48        64
|----------|----------|----------|----------|
|    A     |   FREE   |    C     |   FREE   |
|  16 B    |  16 B    |  16 B    |  16 B    |
|----------|----------|----------|----------|
```

We actually having 16 + 16 = 32, but we can't do it since it doesn't contiguous.

## Source

https://www.geeksforgeeks.org/operating-systems/difference-between-internal-and-external-fragmentation/

