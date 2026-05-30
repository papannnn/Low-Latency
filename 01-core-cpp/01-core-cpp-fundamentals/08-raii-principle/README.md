# RAII Principle

## What is resource

Resource is something that you get from writing a statement or statement, and you release it by writing a statement or statement also.

## Common Resource

### Memory

Acquire
```
p = new T;
```

Release
```
delete p;
```

### Posix File Descriptor

Acquire
```
fp = fopen("filename", "r");
```

Release
```
fclose(fp);
```

### Joinable Threads

Acquire
```
pthread_create(&p, NULL, fn, NULL);
```

Release
```
pthread_join(p, &retVal);
```

### Mutex Locking

Acquire
```
pthread_mutex_lock(&mut);
```

Release
```
pthread_mutex_unlock(&mut);
```

## Resource Usage Issues

### Leak

You lost the access of the resources, and now you can't release it.

### Use After Release

If you use RAII correctly, this can't be happening.

### Double Release

Same here.

## Sample Function

```c++
bool fn(std::mutex & someMutex, SomeDataSource & src) {
    someMutex.lock();
    BufferClass buffer;
    src.readIntoBuffer(buffer);
    buffer.display();
    return true;
}
```

Problem is, we forgot to unlock the mutex, now mutex is locked permanently.

We can fix it by adding unlock
```c++
bool fn(std::mutex & someMutex, SomeDataSource & src) {
    someMutex.lock();
    BufferClass buffer;
    src.readIntoBuffer(buffer);
    buffer.display();
    someMutex.unlock();
    return true;
}
```

But because this is bool return function, I want to return false when it's failing reading the data.

```c++
bool fn(std::mutex & someMutex, SomeDataSource & src) {
    someMutex.lock();
    BufferClass buffer;
    if (!src.readIntoBuffer(buffer)) {
        return false;
    }

    buffer.display();
    someMutex.unlock();
    return true;
}
```

Now we need to add another unlock.

```c++
bool fn(std::mutex & someMutex, SomeDataSource & src) {
    someMutex.lock();
    BufferClass buffer;
    if (!src.readIntoBuffer(buffer)) {
        someMutex.unlock();
        return false;
    }

    buffer.display();
    someMutex.unlock();
    return true;
}
```

What if `readIntoBuffer` can throw an exception?

Add try catch, and unlock on catch, etc.

But, this looks complicated, why we never leak a vector data?

## Object Lifetime

Because in C++, every object has a lifetime, constructor, and destructor.

## RAII

We acquire a resources during a constructor of the object, and we release it on destructor.

## std::lock_guard_lock

Let's see if we can improve our code using this

```c++
bool fn(std::mutex & someMutex, SomeDataSource & src) {
    std::lock_guard_lock{someMutex};
    BufferClass buffer;
    if (!src.readIntoBuffer(buffer)) {
        return false;
    }

    buffer.display();
    return true;
}
```

It's more cleaner, because `someMutex` will be unlocked when `std::lock_guard_lock` got destroyed.

## Source

https://www.youtube.com/watch?v=Rfu06XAhx90