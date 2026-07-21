# Feedback: Lock-Free Stack (Treiber Stack)

Overall this is a solid attempt at a Treiber stack with a tagged pointer for ABA
mitigation. The push/pop CAS loops are structured correctly and the memory
orderings on the CAS are reasonable. The main gaps are around **memory
reclamation** and **thread-safety of the retire list**, which are exactly the
hard parts of lock-free stacks.

---

## Critical

### 1. `_retireList` is not thread-safe
```cpp
std::vector<Node<T>*> _retireList;   // shared, mutated in pop()
...
_retireList.push_back(oldHead._ptr); // called concurrently -> data race
```
`pop()` is meant to be called from multiple threads, but `push_back` on a plain
`std::vector` is not atomic. Concurrent pops will race on the vector's size/
capacity/reallocation and corrupt it (or crash). This is undefined behavior and
defeats the point of the lock-free design.

### 2. Memory is never actually reclaimed while running
Nodes go into `_retireList` and are only `delete`d in the destructor. For a
long-running program this is an **unbounded memory leak** — the list grows with
every `pop()` forever.

The retire list looks like the start of a deferred-reclamation scheme, but the
"when is it safe to free?" logic is missing. That safety question is precisely
what **hazard pointers** or **RCU/epoch-based reclamation** (both listed in your
README topics) exist to answer. Right now nothing frees popped nodes until the
whole stack dies.

Note: because nodes are *never* freed mid-run, the `oldHead._ptr->next`
dereference in `pop()` happens to avoid use-after-free today. The moment you add
real reclamation, that dereference becomes the classic bug hazard pointers
protect against — worth understanding that link.

---

## Important

### 3. Verify the tagged pointer is actually lock-free
```cpp
std::atomic<TaggedPtr<T>> _topPtr{};
```
`TaggedPtr` is `pointer + size_t` = 16 bytes on a 64-bit target. `std::atomic`
on a 16-byte type is only lock-free if the platform has a double-width CAS
(`CMPXCHG16B`, needs `-mcx16` on x86-64). Otherwise the compiler silently falls
back to an internal mutex, and your "lock-free" stack isn't. Add a guard:
```cpp
static_assert(std::atomic<TaggedPtr<T>>::is_always_lock_free,
              "TaggedPtr atomic is not lock-free on this platform");
```

### 4. Unnecessary copies in `push`
```cpp
void push(T param) {
    Node<T> *data = new Node<T>(param);   // copies param again
```
`param` is taken by value (copy #1) and then copied again into the node. Move it:
```cpp
Node<T>* data = new Node<T>(std::move(param));
```
Similarly in `pop`, move the value out since the node is being retired anyway:
```cpp
T val = std::move(oldHead._ptr->data);
```

---

## Minor / polish

- **Missing include guard.** Add `#pragma once` (or guards) to
  `lock_free_stack.hpp`.
- **Non-copyable intent.** Consider explicitly `= delete`ing the copy/move
  ctors and assignment for `ConcurrentStack` to document the intent (the atomic
  member already makes it non-copyable, but being explicit is clearer).
- **`TaggedPtr() = default`** leaves members indeterminate for stack instances.
  You rely on `_topPtr{}` value-initializing to `{nullptr, 0}`, which works, but
  giving explicit default member initializers (`Node<T>* _ptr = nullptr; size_t
  _tag = 0;`) is safer and self-documenting.
- **Exception safety in `pop`.** If the copy `T val = oldHead._ptr->data` threw
  (not for `std::string`, but for a throwing `T`), the node is already unlinked
  but not yet retired → leak + lost value. Moving the value (see #4) sidesteps
  this for most types.
- **`Node`'s constructor** could be `explicit` to avoid accidental implicit
  conversions.
- **`main`** exercises only the single-threaded happy path. To actually validate
  a lock-free stack, add a multi-threaded stress test (N producers + N
  consumers, then assert the total count of pushed vs popped items matches).

---

## Suggested next step
Pick one reclamation strategy and implement it end-to-end:
1. **Hazard pointers** — most instructive for understanding the safety proof.
2. **Epoch-based reclamation** — simpler to get correct, common in practice.

Getting reclamation right (and stress-testing it under TSan:
`-fsanitize=thread`) is the real learning goal here, and it directly fixes
issues #1 and #2.
