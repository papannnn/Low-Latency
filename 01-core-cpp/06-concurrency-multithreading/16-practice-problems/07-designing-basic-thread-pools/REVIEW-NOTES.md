# Designing Basic Thread Pools — Review Notes

Code review feedback for the bounded work-queue + manual worker-thread implementation.

## What Works Well

### Bounded queue blocks correctly

`submit()` waits when the queue is full; `pop()` waits when it is empty. The predicates re-check after wake, so spurious wakeups are handled:

```cpp
_cvProducer.wait(lock, [&]() { return _queue.size() < _size; });
_cvConsumer.wait(lock, [&]() { return _queue.size() > 0; });
```

This matches the monitor pattern from `05-bounded-buffer-problem` and `09-handling-spurious-wakeups-correctly`.

### Two condition variables — right design choice

Separate `_cvProducer` and `_cvConsumer` mean a submitter only wakes a worker (and vice versa). Using `notify_one()` on each side avoids waking every thread when only one can make progress.

### FIFO ordering via `std::deque`

`push_back` on submit and `pop_front` on consume gives fair first-in-first-out task ordering.

### `std::function<void()>` as the task type

Using type-erased callables is the standard choice for a basic thread pool. Callers can submit lambdas, `std::bind` results, or other callable wrappers without templating the queue.

### Bounded capacity enforces backpressure

`WorkQueue(5)` caps in-flight work. When all workers are busy and the queue is full, the producer blocks on `submit()` instead of growing memory without limit — directly addressing the README topic **avoiding unbounded work growth**.

### Multiple workers exercise the queue under contention

Four consumer `jthread`s compete on `pop()`. That is a realistic thread-pool workload and shows the queue can serve more than one worker.

### `std::jthread` for workers

Automatic join at scope exit is consistent with other practice problems in this section and avoids manual `join()` bookkeeping.

### Lambda capture is correct

```cpp
workQueue.submit([=]() {
    std::cout << std::format("Doing {} work", i) << std::endl;
});
```

`[=]` captures `i` by value at submit time, so each stored task prints the intended index even though `i` increments afterward.

---

## Bugs and Gaps to Fix

### 1. No `ThreadPool` abstraction (highest priority)

**Files:** `work_queue.hpp`, `main.cpp`

The README topics are **worker threads**, **task queues**, **submission APIs**, and **shutdown semantics**. Right now you have a `WorkQueue` plus four copy-pasted consumer loops in `main`. A thread pool should own both the queue and the workers:

```cpp
class ThreadPool {
public:
    explicit ThreadPool(std::size_t numWorkers, std::size_t queueCapacity);
    void submit(std::function<void()> task);
    void shutdown();  // stop accepting work, drain, join workers
    ~ThreadPool();
};
```

Workers should live inside the pool and all run the same loop — not duplicated four times in `main`.

### 2. No shutdown semantics — program never terminates

**File:** `main.cpp`

Every thread loops forever (`while (true)`). `main` never sets a shutdown flag, drains remaining tasks, or stops workers. The process only ends on external kill.

**Fix:** Add `std::atomic<bool> _shutdown` (or `std::stop_token`) to the pool. On shutdown:

1. Stop accepting new tasks in `submit()`.
2. Wake all blocked workers with `notify_all()`.
3. Workers exit when the queue is empty **and** shutdown is set.
4. Join all worker threads.

See the drain-on-shutdown pattern in `01-producer-consumer-problem/REVIEW-NOTES.md`.

### 3. `pop()` has no shutdown / poison-pill path

**File:** `work_queue.hpp`

If the producer stops, workers block forever on an empty queue:

```cpp
_cvConsumer.wait(lock, [&]() { return _queue.size() > 0; });
```

There is no way to unblock them without pushing a sentinel task or extending the predicate:

```cpp
return _queue.size() > 0 || _shutdown;
```

After `wait` returns, distinguish “real task” from “shutdown + empty” before blocking again.

### 4. Logging inside the critical section

**File:** `work_queue.hpp`

```cpp
std::cout << "Producer sleep" << std::endl;
_cvProducer.wait(lock, ...);
std::cout << "Producer wakeup" << std::endl;
```

`std::cout` while holding `_mtx` keeps the lock longer than necessary and can stall workers and submitters. Move debug output outside the lock, or use a mutex-guarded `log()` helper.

### 5. Unsynchronized logging across workers

**File:** `main.cpp`

Four consumers print to `std::cout` concurrently. Output will interleave unpredictably.

**Fix:** Route output through a mutex-guarded `log()` helper, as in the sleeping barber solution.

### 6. Missing `#include <chrono>`

**File:** `main.cpp`

`std::chrono::seconds` is used but not included. The file compiles today only because another header pulls `<chrono>` in transitively.

**Fix:** Add `#include <chrono>` explicitly.

### 7. Redundant `if` before `wait` (style, not a bug)

**File:** `work_queue.hpp`

The outer check before `wait` is unnecessary when the predicate already encodes the condition:

```cpp
// Current
if (_queue.size() >= _size) {
    _cvProducer.wait(lock, [&]() { return _queue.size() < _size; });
}

// Idiomatic — predicate covers both initial check and spurious wakeups
_cvProducer.wait(lock, [&]() { return _queue.size() < _size; });
```

Same applies to `pop()`.

### 8. `_size` type mismatch

**File:** `work_queue.hpp`

`_size` is `int` while `_queue.size()` returns `size_t`. The compiler warns on signed/unsigned comparison.

**Fix:** Use `std::size_t` for capacity.

### 9. `WorkQueue` is a synchronization object — make ownership explicit

**File:** `work_queue.hpp`

The class holds `std::mutex` and `std::condition_variable`, so copy is already deleted implicitly. Worth stating explicitly:

```cpp
WorkQueue(const WorkQueue&) = delete;
WorkQueue& operator=(const WorkQueue&) = delete;
```

### 10. Duplicated worker loops in `main`

**File:** `main.cpp`

The same `while (true) { pop(); f(); sleep; }` block appears four times. Once you introduce `ThreadPool`, a single worker function should be spawned `N` times inside the constructor.

---

## Design Notes

| Topic | Current choice | Target for this exercise |
|-------|----------------|--------------------------|
| Queue | `WorkQueue` with `submit` / `pop` | Internal detail of `ThreadPool` |
| Workers | Manual `jthread`s in `main` | Owned and started by `ThreadPool` |
| Submission API | `workQueue.submit(lambda)` | `pool.submit(lambda)` or `pool.enqueue(...)` |
| Shutdown | None | `shutdown()` + drain + join |
| Backpressure | Bounded `submit()` blocks when full | Keep — this is correct |
| Signaling | Two CVs + `notify_one` | Add `notify_all` on shutdown |
| Wait pattern | `if` + `wait` with predicate | Predicate-only `wait` (idiomatic) |
| Task type | `std::function<void()>` | Good; optional follow-up: `std::future` via `std::packaged_task` |
| Logging | Raw `cout` in queue and workers | Mutex-guarded `log()`; never under queue mutex |

---

## Thread Pool Flow (Current vs Desired)

```mermaid
sequenceDiagram
    participant M as main
    participant Q as WorkQueue
    participant W as Workers (x4)

    M->>Q: submit(task) [blocks if full]
    Q->>W: notify_one(_cvConsumer)
    W->>Q: pop() [blocks if empty]
    W->>W: task()
    W->>Q: notify_one(_cvProducer)

    Note over M,W: Desired: ThreadPool owns Q + workers;<br/>shutdown drains queue and joins all threads
```

**Current:** Bounded queue works; workers and lifecycle live in `main`; no clean exit.

**Desired:** `ThreadPool` encapsulates queue + worker loop + shutdown; `main` only submits work and calls `shutdown()`.

---

## Follow-Up Exercises

1. **Introduce `ThreadPool`** — Move worker loops into the class; expose `submit()` and `shutdown()`; `main` submits a fixed number of tasks then shuts down.

2. **Graceful shutdown** — `submit()` rejects or no-ops after shutdown; workers drain until empty + shutdown; all threads join; `main` prints a summary and exits.

3. **`std::packaged_task` + `std::future`** — `template<typename F> auto submit(F&& f) -> std::future<...>` so callers can retrieve results.

4. **Bounded workload demo** — Submit exactly 100 tasks; verify all complete; program exits cleanly.

5. **Backpressure demo** — One fast submitter, slow workers, capacity 5; log when `submit()` blocks to make backpressure visible.

6. **`try_submit`** — Non-blocking variant returning `bool` for callers that should not wait on a full queue.

7. **Metrics** — Track queue high-water mark, tasks completed, and time spent blocked; print at shutdown.

8. **Compare with `std::async`** — Same workload via `std::async` vs your pool; discuss when a pool wins (amortized thread creation, bounded memory).

9. **Exception safety** — Task throws inside a worker; ensure the pool keeps running and optionally logs the error.

10. **Move semantics** — Use `submit(std::function<void()>&&)` or a template `emplace` overload to avoid extra copies of heavy callables.

---

## Verdict

The **synchronization core is solid**: mutex + two condition variables, predicate-based `wait`, `notify_one`, FIFO `deque`, and bounded capacity implement a correct blocking work queue. Backpressure on a full queue and multi-worker contention are both in place.

The main gaps are **architectural** — this is a bounded queue demo, not yet a thread pool. Worker lifecycle, submission API, and shutdown semantics still live outside the abstraction or are missing entirely. Secondary issues: **no termination**, **logging under / across locks**, **missing `<chrono>`**, **signed capacity type**, and **duplicated worker code in `main`**.

Priority fixes:

1. Wrap queue + workers in a `ThreadPool` class with `submit()` and `shutdown()`
2. Implement drain-on-shutdown so workers exit and the program terminates
3. Move debug logging out of the queue mutex; guard worker output
4. Add `#include <chrono>`; use `std::size_t` for capacity
5. Collapse worker loops into one function spawned `N` times inside the pool
