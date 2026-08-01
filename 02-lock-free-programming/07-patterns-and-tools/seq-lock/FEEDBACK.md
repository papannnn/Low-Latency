# Feedback on Seq Lock

The **shape** of this seqlock is right, and that's worth saying first. Odd sequence means "write in progress," the writer bumps the counter twice around the payload, the reader snapshots the counter, copies, and re-checks. That's the algorithm. The `store()` side in particular is a faithful reproduction of the reference implementation you linked, down to the `std::atomic_signal_fence` placement, and the cache-line separation between the payload and the counter is a detail most first attempts skip.

But `load()` has a **real, reproducible bug**: it returns torn data. This is not a theoretical memory-model objection — I built your header against a 16-word payload on this machine (Apple clang 21, arm64, `-O2`, C++20) and the reader observed torn snapshots on every run:

```
reads=1474092  tears=119
reads=1244455  tears=301
reads= 652150  tears=69
```

A "tear" here means the reader got back a value where the 8 words disagreed with each other — half the old snapshot, half the new one. Exactly the failure the sequence counter exists to prevent.

The reason you have never seen it is that `seq_lock_main.cpp` uses `SeqLockData<size_t>`. A single word cannot tear, so the test would pass identically against a class with no synchronization at all. **The bug is in the code and the test is blind to it.** Worse, ThreadSanitizer does not report it either (details in point 5) — so neither of the two things you would normally reach for to gain confidence here would have told you anything.

Below is what's good, then what to fix, with the diagnosis for the tearing bug in point 1.

## 👍 What's Good

1. **The versioning scheme itself is correct.** Even counter means stable, odd means in-flight, and the writer's `+1` / `+2` pair means *any* concurrent write is detectable by a reader that samples the counter twice. That core insight — you don't need to lock readers out, you just need them to notice they raced — is the whole pattern, and you've got it.
2. **The writer is genuinely wait-free.** `store()` has no loop, no CAS, no backoff. It executes a bounded number of instructions and returns regardless of what any reader is doing. Readers cannot block, delay, or starve the writer. That asymmetry is the entire reason a seqlock is chosen over a `shared_mutex` for market-data-style workloads, and your implementation preserves it exactly.
3. **The `store()` side matches the reference implementation.** Loading `_seq` with `relaxed` is correct precisely *because* the writer is the only one who mutates it — no other thread can change the value between your load and your store, so there is nothing to synchronize with. Getting that right (rather than reflexively reaching for `fetch_add`) shows you reasoned about it rather than copying.
4. **False sharing is handled.** Putting `_data` and `_seq` on separate cache lines via `alignas(std::hardware_destructive_interference_size)` matters a lot here: the writer stores to both on every update, and a reader spinning on `_seq` would otherwise pull the payload's line into shared state and fight the writer for it on every iteration. Most first implementations miss this entirely.
5. **You cached the sequence number in a local.** `size_t seq = _seq.load(...)` and then comparing against that local is required — re-reading `_seq` for the "before" value at comparison time would compare the counter against itself and always succeed. Small thing, easy to get wrong.
6. **The early odd-check is a real optimization.** Skipping the payload copy when you can already see a write in flight avoids a guaranteed-wasted copy. Not required for correctness, but the right instinct.
7. **`release`/`acquire` rather than `seq_cst`.** You didn't reach for the default. On arm64 that's the difference between `stlr`/`ldar` and full `dmb ish` barriers.
8. **The class is non-copyable, correctly.** `std::atomic` is non-copyable, so `SeqLockData` inherits that. Accidental rather than designed, but the outcome is right — copying a live seqlock would be meaningless.

## 🛠️ What Needs to be Improved

### 1. The bug: `load()` is missing an acquire fence before the second sequence read

Here is the current read loop:

```cpp
size_t seq = _seq.load(std::memory_order_acquire);   // (A)
// ...
data = _data;                                        // (B) plain, non-atomic reads
if (seq == _seq.load(std::memory_order_acquire)) {   // (C)
    break;
}
```

The intent is "(B) happens between (A) and (C), so if the counter didn't change, the payload is stable." The first half holds: the acquire on (A) prevents (B) from being hoisted above it. **The second half does not.**

An acquire load orders itself *before* everything that follows it. It says nothing about what came before. So nothing stops the payload reads at (B) from being reordered to execute *after* the counter read at (C) — the CPU is free to satisfy (C) from cache while the loads for (B) are still in flight. When that happens the reader validates a counter, then reads the payload while the writer is midway through overwriting it, and returns the mixture. That is precisely the 60–300 torn reads per run in the numbers above.

You need a **load-load barrier**: prior loads must complete before the subsequent counter read. That is `std::atomic_thread_fence(std::memory_order_acquire)`.

I tested four variants against the same 20-million-write stress loop to confirm the diagnosis:

| Variant | Result |
| --- | --- |
| Your current `load()` | tears |
| Plus `atomic_signal_fence` around the copy (matching the reference impl) | **still tears** |
| Plus `atomic_thread_fence(acquire)` before the second read | 0 tears |
| Also `atomic_thread_fence(release)` in `store()` | 0 tears |

The second row is the important one, and it's a trap worth understanding: **`std::atomic_signal_fence` is a compiler barrier only.** It emits no instruction. It stops the *compiler* from moving code across it, which is why the reference implementation uses it, but it does nothing about the *processor* reordering loads at runtime. On x86 that gap is invisible, because x86's TSO model already forbids load-load reordering — the reference implementation you linked targets x86-64, where its signal fences are sufficient. You are on Apple Silicon, whose weaker model permits exactly this reordering. **A seqlock ported from x86 to ARM without upgrading the fences is silently broken**, and you've reproduced that firsthand.

* **Fix**:

```cpp
T load() const noexcept {
    T data;
    std::size_t seq0, seq1;
    do {
        seq0 = _seq.load(std::memory_order_acquire);
        if (seq0 % 2 != 0) continue;          // write in flight, don't bother copying

        data = _data;

        std::atomic_thread_fence(std::memory_order_acquire);
        seq1 = _seq.load(std::memory_order_relaxed);
    } while (seq0 != seq1 || seq0 % 2 != 0);
    return data;
}
```

The `fence(acquire)` + `relaxed` load is the standard idiom: the fence carries the ordering, so the load itself doesn't need to. I ran this against 30 million writes with a 16-word payload, three times: zero tears, and the reader never observed a value going backwards.

### 2. `store()` has the mirror-image hole

Same reasoning, other direction:

```cpp
_seq.store(seq + 1, std::memory_order_release);   // (A)
_data = std::move(data);                          // (B)
```

A release store orders everything *before* it; it does not prevent (B) from being reordered *ahead* of (A). If the payload write becomes visible before the odd counter does, a reader can sample an even counter, read half-written payload, re-sample the same even counter, and accept it.

I could not reproduce this one — the fourth row of the table above is indistinguishable from the third in practice, most likely because `stlr` followed closely by plain stores rarely reorders on this core. But "I couldn't make it fail" is not the standard you want after point 1 just demonstrated what unreproduced-until-now looks like.

* **Fix**: make the counter increment `relaxed` and put a real release fence after it, so the fence (not the store) provides the ordering:

```cpp
void store(const T& data) noexcept {
    std::size_t seq = _seq.load(std::memory_order_relaxed);
    _seq.store(seq + 1, std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_release);   // odd counter lands before payload

    _data = data;

    _seq.store(seq + 2, std::memory_order_release);        // payload lands before even counter
}
```

The trailing `release` store is already correct and needs no fence — release is exactly the "everything before me is visible first" guarantee you want there.

### 3. `_data` is never initialized

```cpp
alignas(...) T _data;   // default-initialized: indeterminate for scalars
```

A `load()` before the first `store()` reads an indeterminate value. This isn't hypothetical; two default-constructed `SeqLockData<size_t>` objects printed:

```
load() before any store(): 0 44
```

The `0` is luck. The `44` is stack garbage. In `seq_lock_main.cpp` both consumers start before the producer's first `store()` lands, so this is live in your own demo.

* **Fix**: `T _data{};`

### 4. Nothing stops you from instantiating this with a type that will crash

A seqlock reads the payload while it may be concurrently overwritten. That is only survivable for **trivially copyable** types, where the worst case is a nonsense value you then discard. With a non-trivially-copyable type, the copy runs a real constructor over half-written bytes.

`SeqLockData<std::string>` compiles cleanly against your header today, with no warnings under `-Wall -Wextra`. At runtime, `data = _data` on a torn `std::string` reads a garbage pointer and length — segfault, or a `free()` on an address that was never allocated.

* **Fix**: reject it at compile time, where the error is one line instead of a two-hour debugging session.

```cpp
static_assert(std::is_trivially_copyable_v<T>,
              "SeqLock reads the payload concurrently with writes; T must be trivially copyable");
```

While you're there: `T data;` in `load()` also requires `T` to be default-constructible, and `data = _data` requires copy-assignment. The `static_assert` makes those requirements explicit instead of surfacing as template error walls.

### 5. The data race is real UB — and TSan will not save you

Worth being clear-eyed about: even with points 1 and 2 fixed, `data = _data` racing with `_data = data` is a data race by the letter of the standard, and therefore undefined behaviour. The fences make it work on real hardware; they do not make it conforming. Every production seqlock lives with this, so it's a legitimate engineering choice — but it should be a documented decision, not an oversight.

The part that surprised me, and that's the most useful thing in this review after point 1:

```
plain unsynchronized counter (control):  2 data races reported
your seqlock, 12.4 million reads:        0 data races reported
```

Same binary settings, same TSan. **TSan reported nothing on code I had already proven tears.** The reason is that TSan is a happens-before detector: it sees the release stores and acquire loads on `_seq`, builds a synchronization edge from them, and concludes the payload accesses are ordered — even though the edge isn't actually strong enough to order them. The `_seq` atomics essentially launder the race past the detector.

The takeaway to write down: **for this pattern, a clean TSan run is not evidence of correctness.** Which is exactly why the stress test in point 6 matters — a tearing check with a multi-word payload found in seconds what TSan missed across 12 million reads.

If you want a version that is *actually* free of UB, the standard-conforming approach is to make the payload an array of `std::atomic` and read/write each element with `memory_order_relaxed`. Relaxed atomic accesses can race legally, the codegen is usually identical to plain loads and stores, and the fences still provide the ordering. It's worth building once as a comparison, if only to confirm the generated assembly matches.

### 6. `seq_lock_main.cpp` cannot fail, and floods the terminal doing it

I ran it. It produced **2,817,827 lines / 5.4 MB of output** in five seconds, to communicate five values.

Every problem in it compounds the same way — nothing here can distinguish a working seqlock from a broken one:

* **`size_t` can't tear.** A single aligned word is never half-written on any platform you'll target. The payload has to be at least two words for the test to mean anything. This alone is why the point-1 bug survived.
* **No assertions.** Nothing verifies any property. The output is for a human to squint at, and no human is reading 2.8 million lines.
* **Five writes in five seconds.** `sleep_for(1s)` in the writer means the race window is essentially never open. Stress tests need millions of iterations, not a pause.
* **The consumers spin on `std::cout`.** I/O dominates so heavily that the readers barely touch the seqlock, which further shrinks the window you're trying to hit.
* **Output is interleaved and garbled.** Sorting the output showed lines reading `00`, `11`, `22`, `33`, `44` — two threads' digits landing on one line. `std::cout` is race-free but each `<<` is a separate call. Use `std::osyncstream` (C++20) if you keep printing at all.
* **`finished` is checked only at the top of the loop**, so both consumers can block one extra `load()` past the end. Harmless here, but the shutdown handshake is sloppy.

* **Fix**: make the payload self-validating. The classic seqlock test writes N copies of the same number and asserts the reader sees N copies of *some* number:

```cpp
#include <array>
#include <atomic>
#include <cassert>
#include <print>
#include <thread>
#include "seq_lock.hpp"

using Payload = std::array<std::size_t, 16>;   // multi-word: CAN tear

int main() {
    SeqLockData<Payload> sl;
    std::atomic<bool> done{false};
    std::atomic<std::size_t> tears{0}, reads{0}, backwards{0};

    std::jthread writer([&] {
        for (std::size_t i = 1; i <= 30'000'000; i++) {
            Payload p;
            p.fill(i);                          // invariant: all elements equal
            sl.store(p);
        }
        done.store(true, std::memory_order_release);
    });

    std::jthread reader([&] {
        std::size_t last = 0;
        while (!done.load(std::memory_order_acquire)) {
            Payload p = sl.load();
            reads.fetch_add(1, std::memory_order_relaxed);
            for (std::size_t v : p)
                if (v != p[0]) { tears.fetch_add(1, std::memory_order_relaxed); break; }
            if (p[0] < last) backwards.fetch_add(1, std::memory_order_relaxed);
            last = p[0];                        // snapshots must be monotonic
        }
    });

    writer.join();
    reader.join();
    std::println("reads={} tears={} backwards={}", reads.load(), tears.load(), backwards.load());
    assert(tears.load() == 0);
    assert(backwards.load() == 0);
}
```

Two invariants, both cheap and both meaningful: elements must agree with each other (no tearing), and successive snapshots must not go backwards (no stale publication). Run it against your current header first and watch it fail — that failure is the most valuable thing in this folder. Then apply the fixes and watch it pass.

### 7. Single-writer is a hard requirement and it isn't stated anywhere

`store()` does load-then-store on `_seq` without an atomic RMW. Two writers can read the same `seq`, both write `seq+1`, both write the payload on top of each other, and both write `seq+2` — leaving the counter **even** while the payload is shredded. Readers will happily accept it. There is no assertion, no comment, and no README line saying so.

* **Fix**: document it prominently, and consider a debug-build guard that catches violations:

```cpp
// Single writer only. Concurrent store() calls corrupt the payload silently.
assert(_seq.load(std::memory_order_relaxed) % 2 == 0 && "concurrent store() detected");
```

If you ever do need multiple writers, the answer is a mutex among writers only — readers stay lock-free. That is a genuinely good follow-up exercise, because it makes the point that "lock-free" is a per-role property, not a per-data-structure one.

### 8. Reader progress: `load()` is not lock-free, and the spin has no backoff

Worth getting the terminology exactly right in your notes, because seqlocks are a common source of confusion:

* The **writer is wait-free** — bounded steps, always.
* The **reader is not lock-free.** A reader can be starved indefinitely by a writer that keeps updating. It makes no system-wide progress guarantee; formally it's only obstruction-free. A seqlock is best described as "readers never block the writer," which is a different and weaker claim than "lock-free."

The practical consequence in your code is the bare `continue` on an odd counter: a tight retry loop with no pause, burning issue slots and hammering the counter's cache line while the writer is trying to update it.

* **Fix**: add a spin hint. On arm64 that's `__builtin_arm_yield()` (the `yield` instruction); on x86 it's `_mm_pause()`. `std::this_thread::yield()` is the portable but much heavier option — it's a syscall, which is the wrong order of magnitude for a window this short.

### 9. Header hygiene

* **`<iostream>` is included and never used.** It's the heaviest header in the standard library and it injects a static initializer into every TU that includes it. Delete it.
* **Missing includes**: you use `size_t` but never include `<cstddef>`; it only compiles because another header drags it in. Also use `std::size_t`, not the global `size_t` — the unqualified name is only guaranteed by the C headers. `<type_traits>` will be needed for the `static_assert` in point 4.
* **`load()` should be `const noexcept`**, and `store()` should take `const T&` rather than by-value-plus-`std::move`. Once `T` is constrained to trivially copyable, the move buys nothing over a copy, and a non-const `load()` prevents readers from holding a `const SeqLockData&` — which is exactly what a reader *should* hold.
* **`std::hardware_destructive_interference_size` has portability caveats.** GCC warns on it (`-Winterference-size`) because its value is baked into your ABI, and it was absent from libc++ for years. It works on this toolchain, but a `constexpr std::size_t kCacheLine = 64;` with a comment is what most production code does. Worth a note either way.
* **Naming**: `_data` / `_seq` use a leading underscore. Legal inside a class, but leading-underscore identifiers are reserved at namespace scope, so `data_` / `seq_` is the habit that never bites you. Minor, but be consistent across the repo.

### 10. The README should carry the reasoning

Right now it's a title and two links. The links are the right ones, but after debugging point 1 you know things that aren't in either of them. Worth writing down in your own words:

* **What a seqlock buys you**: optimistic reads with zero writer-visible cost. No reader state to update means no cache line ping-pong from the read side — the reason this pattern dominates in market data feeds, `clock_gettime` in the Linux vDSO, and anywhere one publisher feeds many latency-sensitive consumers.
* **What it costs**: readers can starve, readers can't be preempted mid-copy without wasting the attempt, and it only works for trivially copyable payloads. It is not a general-purpose replacement for `shared_mutex`.
* **Why the fences are where they are** — one line each for the four ordering constraints. This is the part you now understand better than most people who have read about seqlocks, because you measured it.
* **The x86-vs-ARM trap from point 1.** This is the single most valuable note in the folder. A correct-looking seqlock copied from an x86 codebase tears on ARM, and neither the test suite nor TSan will tell you.
* **Required standard**: C++17 for `hardware_destructive_interference_size`, C++20 for `std::jthread`.

### 11. Build artifacts and no build file

`a.out` and `seq_lock.hpp.pch` are sitting in the folder. Add a `.gitignore`, and a `Makefile` matching the convention in `01-core-cpp/02-memory-management/03-smart-pointers/` — with targets for a plain `-O2` build and a TSan build, so the stress test is one `make` away.

## Suggested Priority

1. **Run the stress test from point 6 against the current header** — before changing anything. Seeing your own code tear is worth more than reading this file.
2. **Add the acquire fence in `load()` and the release fence in `store()`** (points 1 and 2), then re-run and watch the tear count go to zero. This is the fix.
3. **Initialize `_data{}` and add the `static_assert`** (points 3 and 4) — two lines, closes a garbage read and a crash-on-`std::string`.
4. **Write down the TSan false negative** (point 5). It's the most surprising result here and the easiest to forget.
5. **Document the single-writer requirement, add the spin hint, clean up the header** (points 7, 8, 9).
6. **Expand the README** (point 10) while the debugging is fresh.

Once that's solid, the natural next step within this chapter is the multi-writer variant from point 7 — a mutex between writers with readers still unblocked — because it forces you to articulate which role gets which progress guarantee. After that, the memory-reclamation topics in the parent README (hazard pointers, epoch-based reclamation) are the real step up. A seqlock sidesteps reclamation entirely by never freeing anything: the payload lives in place forever and readers retry. Those techniques solve the harder problem of publishing new allocations repeatedly and knowing when it's safe to free the old ones.
