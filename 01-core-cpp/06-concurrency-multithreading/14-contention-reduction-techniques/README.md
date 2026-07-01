# Contention Reduction Techniques

## Contention

Is when multiple thread just doing nothing waiting the lock become available, making the performance degrades because they're doing nothing.

## Use Shared Mutex

We can use shared mutex to differentiate read and write activity, so multiple read is allowed and improving the performance, especially if you have more read than write.

## Use fine grain over Coarse Grain for locking granularity

Coarse-grain locking is one big mutex around a whole data structure

Fine-grain locking puts smaller mutexes deep inside the structure (per bucket in a hash map, per node in a list, per shard, etc.)

## Reduce Lock Frequency

Instead of

```c++
for (int x : values)
{
    std::lock_guard lock(mutex);
    shared.push_back(x);
}
```

Do

```c++
std::vector<int> local;

for (int x : values)
    local.push_back(x);

{
    std::lock_guard lock(mutex);
    shared.insert(shared.end(), local.begin(), local.end());
}
```

## Source

https://towardsdev.com/lock-contention-cpp-low-latency-guide-0c452c8acd1b

https://www.geeksforgeeks.org/cpp/thread_local-storage-in-cpp-11/