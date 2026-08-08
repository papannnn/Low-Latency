# Hazard Pointer Implementation Review

This directory contains an educational hazard-pointer domain with:

- a fixed number of owner slots;
- two protected pointers per owner;
- a fixed-size retired-pointer list;
- one scanner at a time; and
- cache-line padding around frequently accessed atomics.

The design demonstrates the main purpose of hazard pointers: defer deletion of an
object while another thread may still access it. It is a useful prototype, but
the current implementation is **not safe for production use yet**.

## What Is Good

1. **The responsibilities are separated clearly.** `HazardPointer` owns the
   shared domain, while `HazardPointerOwner` represents a thread's participation
   in that domain.
2. **Owner cleanup uses RAII.** Destroying an owner clears its hazard slots and
   releases its owner slot.
3. **Reclamation checks every hazard slot before deleting a retired pointer.**
   This captures the central idea of the hazard-pointer algorithm.
4. **Only one thread scans the retired list at a time.** `_doDeletion` avoids
   concurrent scanners deleting the same entry.
5. **The domain itself cannot be copied.** Deleting its copy constructor and
   copy assignment operator prevents accidental duplication of shared state.
6. **False sharing was considered.** Padding the atomics shows awareness that
   cache-line contention matters in concurrent algorithms.
7. **Memory orders are written explicitly.** Even though the ordering still
   needs revision, making it visible is valuable for learning and review.

## What Should Be Improved

### 1. Fix the data race and hazard-publication protocol in the example

`hazard_pointer_main.cpp` reads and writes `val` concurrently as a plain
pointer. That is a C++ data race and therefore undefined behavior. Both threads
can also retire the same pointer, overwrite newly allocated pointers, and leak
memory.

The pointer in the protected data structure must be atomic. A reader must:

1. load the shared pointer;
2. publish it in a hazard slot;
3. load the shared pointer again;
4. retry if it changed; and
5. only then dereference the object.

The thread that successfully removes an object from the shared data structure
must retire it exactly once. Publishing a hazard after an ordinary pointer read
is not sufficient.

### 2. Handle owner-slot exhaustion

`make_owner()` leaves `idx` equal to zero when every owner slot is occupied. It
then returns a second owner for slot zero. Either owner can consequently clear
the other owner's hazards, allowing an object to be deleted while it is still
in use.

Return an error (for example, `std::optional`), throw an exception, or use a
well-defined waiting policy when no slot is available. Also add
`static_assert(S > 0)`.

### 3. Make `HazardPointerOwner` move-only

The owner is currently implicitly copyable. Copies refer to the same slot, so
the first copy destroyed resets the slot while another copy may still depend on
it. Delete copy construction and copy assignment, then implement ownership-
transferring move operations with an invalid state for the moved-from object.
The hazard-pointer domain must also outlive every owner.

### 4. Correct `protect()` and report failure

After the first `compare_exchange_strong` fails, it replaces `expected` with the
actual value of slot zero. The same `expected` is then passed to the CAS for
slot one. As a result, the second CAS normally fails even when slot one is
empty, and in some states it could replace an already protected pointer.

Reset `expected` to `nullptr` before the second CAS. More importantly,
`protect()` must not silently return when both slots are occupied. Return a
slot/guard or an explicit failure result. A caller that assumes a failed
publication succeeded can dereference freed memory.

An RAII `HazardGuard` would make this safer: reserve one slot on construction,
publish through it, and clear that exact slot on destruction. It would also
avoid the current ambiguous behavior when the same pointer is protected more
than once and one call to `unprotect()` clears every matching slot.

### 5. Strengthen and document the memory-ordering proof

Acquire/release operations on the hazard slot and the source pointer do not, by
themselves, prevent this outcome:

- the reader misses the remover's update to the source pointer; and
- the remover misses the reader's hazard publication.

That outcome can permit premature deletion. For a first correct version, use
`std::memory_order_seq_cst` for hazard publication, hazard scanning, and the
relevant source-pointer operations. Optimize to weaker orders only after
writing down a complete C++ memory-model proof. Hardware behavior on one
platform is not enough to establish portable correctness.

### 6. Replace the fixed retired list or handle a full list

`retire()` loops forever when all `S * 8` entries are occupied. This can consume
an entire CPU while making no progress. The capacity is also unrelated to the
number of objects that readers may keep protected.

A conventional design gives each owner a local retired list and scans it after
a threshold is reached. If fixed storage is intentional for the exercise,
`retire()` should at least expose failure or use a documented backoff policy.

The use of `std::set`, general allocation, and arbitrary `T` destructors during
scanning also means the reclamation path should not be described as fully
lock-free.

### 7. Make reclamation and shutdown rules explicit

- Skip `nullptr` entries during a scan.
- Define duplicate retirement as a caller error and detect it in debug builds;
  trying to recover from double retirement can still lead to double deletion
  under concurrency.
- Ensure a pointer inserted after a scanner has passed its slot will eventually
  trigger another scan.
- Release `_doDeletion` with an RAII guard so an exception cannot leave
  reclamation permanently disabled.
- At domain destruction, require that all owners and data-structure operations
  have finished. Verify that no hazards remain and reclaim all remaining
  retired entries.
- Explicitly initialize every atomic, including `_retireList`, rather than
  relying on language-version-specific default initialization.

### 8. Improve portability

`std::thread::id` is trivially copyable, so using it with `std::atomic` is
permitted. The resulting atomic is not guaranteed to be lock-free, however. If
lock-freedom is a requirement, check the implementation's guarantee or use an
atomic claimed/free flag to reserve each slot. A thread ID can be retained
separately for debug checks if desired.

`std::hardware_destructive_interference_size` is also implementation-dependent.
Consider a documented fallback when portability is a goal.

### 9. Add correctness-focused tests

The sample currently prints messages but does not verify reclamation. Add tests
for:

- owner-slot exhaustion;
- owner move semantics;
- both hazard slots being occupied;
- an object not being destroyed while protected;
- eventual destruction after its hazard is cleared;
- concurrent removal with exactly-once retirement;
- a full retired list; and
- many randomized reader/remover iterations.

Run the stress tests with ThreadSanitizer, AddressSanitizer, and Undefined
Behavior Sanitizer. Sanitizers cannot prove a lock-free algorithm correct, but
they are very useful for exposing the current data race, leaks, double frees,
and use-after-free errors.

## Suggested Implementation Order

1. Replace the sample's shared pointer with an atomic pointer and implement the
   load-publish-recheck loop.
2. Make owners move-only and make owner acquisition fail safely.
3. Introduce a slot-specific RAII hazard guard.
4. Start with sequentially consistent ordering and validate correctness.
5. Replace the fixed retired array with per-owner retired lists.
6. Add sanitizer-backed stress tests.
7. Only then measure padding and weaker memory-order optimizations.

## Sources

- [Hazard pointers: safe memory reclamation for lock-free objects](https://ieeexplore.ieee.org/document/1291819)
- [A Lock-Free Stack: A Hazard Pointer Implementation](https://www.modernescpp.com/index.php/a-lock-free-stack-a-hazard-pointer-implementation/)
- [Hazard pointer video](https://www.youtube.com/watch?v=VKbfinz6D04)
