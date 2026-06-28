# Lock Guards and Unique Locks

## `std::lock_guard`

Let's see the code without `std::lock_guard` first

```c++
std::mutex m1;

void task() {
    m1.lock();

    // Do something in here

    m1.unlock();
}

int main () {
    thread t1(task);
    thread t2(task);
    t1.join();
    t2.join();
}
```

As you can see, you need explicitly call `lock` and `unlock`. What if you forgot to do 1 of them? It will introduce defect on your code.

Now let's see the code using `std::lock_guard`

```c++
std::mutex m1;

void task() {
    std::lock_guard<mutex> lock(m1);

    // Do something in here
}

int main () {
    thread t1(task);
    thread t2(task);
    t1.join();
    t2.join();
}
```

You just need that 1 line. It's because on constructor, `lock_guard` will call the `lock` on our mutex, and on destruction of the object, the `lock_guard` will call `unlock`.

## `std::unique_lock`

`std::unique_lock` is almost the same like `std::lock_guard`, but more flexibility.

You can manually `lock()` & `unlock()` using `std::unique_lock`

```c++
void safeIncrementWithControl(int& counter, std::mutex& mtx) {
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);  // Deferred locking
    do_something();
    lock.lock();
    ++counter;  // Safe modification
    lock.unlock();  // Optionally unlock before scope ends, even if you dont unlock its RAII :)
}
```

## `std::lock`

It's a function to lock multiple mutex at once.

It doesn't unlock mutex automatically, so you need to use `std::lock_guard` or `std::unique_lock` and using `std::adopt_lock` strategy.

```c++
void safeTransfer(int& from, int& to, std::mutex& mtx1, std::mutex& mtx2) {
    std::lock(mtx1, mtx2);  // Locks both without deadlock risk
    std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock);
    from -= 10;
    to += 10;
}
```

## `std::scoped_lock`

Providing RAII styled lock for multiple mutex.

```c++
void safeMultiLock(int& shared1, int& shared2, std::mutex& mtx1, std::mutex& mtx2) {
    std::scoped_lock lock(mtx1, mtx2);  // Locks both mutexes safely
    shared1++;
    shared2++;
}
```

## `std::shared_lock`

It allow multiple thread to acquire shared access simultaneously while blocking writer.

```c++
int readData(const std::string& key, const ConcurrentMap& cmap) {
    std::shared_lock<std::shared_mutex> lock(cmap.mutex);
    auto it = cmap.map.find(key);
    return it->second;
}
```

## `std::try_lock`

Attempt to acquire multiple lock without blocking, if any of locks is unavailable, it will fail and unlock any lock that has been acquired.

```c++
void safeTryLock(int& shared1, int& shared2, std::mutex& mtx1, std::mutex& mtx2) {
    // Try to lock both mutexes without blocking
    if (std::try_lock(mtx1, mtx2) == -1) {  // -1 indicates success in locking both
        std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock);  // Adopt the first lock
        std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock);  // Adopt the second lock
        shared1++;
        shared2++;
    } else {
        // If either mutex couldn't be locked, handle failure here
        std::cout << "Could not acquire both locks, skipping update." << std::endl;
    }
}
```

## Lock tags (Locking strategies)

### `std::defer_lock`

`std::defer_lock`: Creates the lock object without immediately locking the mutex. Useful when you want to lock later or use std::lock() to lock multiple mutexes

```c++
void processWithDefer(std::mutex& mtx) {
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock); // Doesn't lock yet
    
    // Do some preparation work without the lock
    prepareData();
    
    lock.lock();   // Now explicitly lock
    // Critical section
    processData();
    lock.unlock(); // Explicitly unlock if needed
}
```

### `std::try_to_lock`

`std::try_to_lock`: Attempts to lock without blocking. If the lock can't be acquired immediately, continues without waiting useful when you don't want to wait for a lock.

```c++
bool processIfAvailable(std::mutex& mtx, int& shared_data) {
    std::unique_lock<std::mutex> lock(mtx, std::try_to_lock);
    
    if (!lock.owns_lock()) {
        // Mutex was locked, return without waiting
        return false;
    }
    
    // We got the lock, safe to modify data
    shared_data++;
    return true;
}
```

### `std::adopt_lock`

`std::adopt_lock`: Assumes the mutex is already locked and adopts it. Useful when the mutex was locked manually or by `std::lock()`

```c++
void transferWithAdopt(int& from, int& to, std::mutex& mtx1, std::mutex& mtx2) {
    // First manually lock both mutexes
    std::lock(mtx1, mtx2);  // Locks both without deadlock
    
    // Create guards that adopt existing locks
    std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock);
    
    // Safe to perform transfer
    from -= 100;
    to += 100;
    // Guards automatically release locks when destroyed
}
```

## Source

https://www.youtube.com/watch?v=ACoYnEzjEz4

https://www.youtube.com/watch?v=NsqqWDQHldo

https://jenniferchukwu.com/posts/LockingMechanisms