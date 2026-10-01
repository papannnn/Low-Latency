# Cache Lines

## What is that

When CPU read data, it will not only read that bytes only, it will read whole cache line, most CPU architecture L1 cache has 64 bytes.

So assuming there's an array of int with size 16

```c++
int arr[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};

foo(arr[0]); // Access the array
```

Assuming int size is 4 bytes. When CPU access arr[0], and cache line is 64 bytes.

That means 64 bytes / 4 bytes = 16 integer will be fetched into the cache lines.

Next time when CPU want to access arr[1], it doesn't need to go to memory anymore, it simply just go to L1 cache because whole 16 integer already got cached.

This is called spatial locality

## Spatial Locality

If you access data A, there's a high chance you will access data around A.

## Memory Access Pattern

When you loop the array sequentially, the performance is better than you access the array randomly.

It's because when you access array sequentially, hardware will also prefetch the memory for you. Making the performance is more faster.

When you accessing things randomly, hardware can't predict where you gonna access the memory, making the prefetcher not working optimally.


## Source

https://youtu.be/Q2e-5DreGb0?si=XftZoL5aWnPZBMQ8
