# Lock-Free and Wait-Free Definitions

If ranked by worst to best

- Blocking
- Starvation Free
- Obstruction Free
- Lock Free
- Wait Free
- Wait Free Bounded
- Wait-Free Population Oblivious

### Blocking

Basically, almost everything with a lock is Blocking.

In the worst case, a thread holding the lock may be put to sleep and thus block every other thread (waiting on that lock) preventing them from making any progress.

```java
AtomicInteger lock = new AtomicInteger(0);
public void funcBlocking() {
         while (!lock.compareAndSet(0, 1)) {
               Thread.yield();
        }
}
```

### Starvation Free

As long as one thread is in the critical section, then some other thread that wants to enter in the critical section will eventually succeed

Example is ticket lock

```c
void ticket_mutex_init(ticket_mutex_t * self)
{
    atomic_store(&self->ingress, 0);
    atomic_store(&self->egress, 0);
}


void ticket_mutex_destroy(ticket_mutex_t * self)
{
    // kind of unnecessary, but oh well
    atomic_store(&self->ingress, 0);
    atomic_store(&self->egress, 0);
}


/*
 * Locks the mutex
 * Progress Condition: Blocking
 */
void ticket_mutex_lock(ticket_mutex_t * self)
{
    long lingress = atomic_fetch_add(&self->ingress, 1);
    while (lingress != atomic_load(&self->egress)) {
        sched_yield();  // Replace this with thrd_yield() if you use <threads.h>
    }
    // This thread has acquired the lock on the mutex
}


/*
 * Unlocks the mutex
 * Progress Condition: Wait-Free Population Oblivious
 *
 * We could do a simple atomic_fetch_add(egress, 1) but it is faster to do
 * the relaxed load followed by the store with release barrier.
 * Notice that the load can be relaxed because the thread did an acquire
 * barrier when it read the "ingress" with the atomic_fetch_add() back in
 * ticket_mutex_lock() (or the acquire on reading "egress" at a second try),
 * and we have the guarantee that "egress" has not changed since then.
 */
void ticket_mutex_unlock(ticket_mutex_t * self)
{
    long legress = atomic_load_explicit(&self->egress, memory_order_relaxed);
    atom
```

### Obstruction Free

A function is Obstruction-Free if, from any point after which it executes in isolation, if finishes in a finite number of steps

Basically, succeeds if uncontended

### Lock Free

Guarantees that at least some thread is doing progress on its work

```java
AtomicInteger atomicVar = new AtomicInteger(0);
public void funcLockFree() {
  int localVar = atomicVar.get();
  while (!atomicVar.compareAndSet(localVar, localVar+1)) {
          localVar = atomicVar.get();
  }
}
```

### Wait Free

Guarantees that every call finishes its execution in a finite number of steps.

### Wait Free Bounded

Guarantees that every call finishes its execution in a finite and bounded number of steps. This bound may depend on the number of threads.

```java
AtomicIntegerArray intArray = new AtomicIntegerArray(MAX_THREADS);
public void funcWaitFreeBounded() {
  for (int i = 0; i < MAX_THREADS ; i++) {
    intArray.set(i, 1);
  }
}
```

### Wait Free Population Oblivious

Wait-Free method whose performance does not depend on the number of active threads is called Wait-Free Population Oblivious.

```c++
atomic<int> counter;
void funcWaitFreeBoundedPopulationOblivious() {
  counter.fetch_add(1);
}
```

## ABA Problem and Solutions

### Hazard Pointers

### Version Counters

### Tagged Pointers


## Source

https://concurrencyfreaks.blogspot.com/2013/05/lock-free-and-wait-free-definition-and.html

https://github.com/pramalhe/ConcurrencyFreaks/blob/master/C11/locks/ticket_mutex.c

https://www.youtube.com/watch?v=5sZo3SrLrGA