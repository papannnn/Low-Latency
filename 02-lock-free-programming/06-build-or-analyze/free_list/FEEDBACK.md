# Feedback on Free List Implementation

Overall, this is a solid attempt at building a custom memory allocator using a doubly-linked free list. You've correctly utilized placement `new`, handled memory coalescing (compaction), and attempted to use modern C++ features like move semantics. 

However, considering this is located in a `02-lock-free-programming` directory, there are significant architectural and implementation issues that need to be addressed, particularly regarding thread safety, memory leaks, and pointer arithmetic.

Here is a breakdown of what's good and what needs to be improved.

## 👍 What's Good

1. **Custom Allocator Concept**: You successfully implemented the core concept of a free list allocator, managing a contiguous block of memory and splitting it for allocations.
2. **Placement New**: You correctly used placement `new` (`new (address) Type(...)`) to construct objects directly in your pre-allocated memory buffer without triggering default heap allocations.
3. **Memory Coalescing**: You included a compaction algorithm in `deallocate` to merge adjacent free blocks, which is crucial for preventing memory fragmentation over time.
4. **Move Semantics**: You attempted to implement the Rule of Five by defining a move constructor and move assignment operator, while explicitly deleting copy constructors to enforce unique ownership of the memory block.

## 🛠️ What Needs to be Improved

### 1. Not Lock-Free (Thread Safety)
Given the directory name (`02-lock-free-programming`), the biggest issue is that **this code is not lock-free, nor is it thread-safe**. 
* It uses standard pointers and standard arithmetic. 
* If multiple threads call `allocate` or `deallocate` simultaneously, data races will occur, leading to corrupted linked lists and segmentation faults.
* **Fix**: To make this lock-free, you need to use `std::atomic` for your pointers and implement Compare-And-Swap (CAS) loops. Lock-free free lists often require techniques like hazard pointers or tagged pointers to avoid the ABA problem.

### 2. Memory Leak in Move Semantics
Your move constructor and move assignment operator transfer `_head`, but they forget to transfer `_validStart` and `_validEnd`.
```cpp
FreeList(FreeList&& freeList): _head(std::exchange(freeList._head, nullptr)) { }
```
Because `_validStart` is not copied, it defaults to `0`. When the moved-to object is destroyed, `~FreeList()` calls `free(0)` (which does nothing), and the actual memory allocated via `malloc` is permanently leaked.
* **Fix**: Ensure you transfer all member variables and zero them out in the source object:
```cpp
FreeList(FreeList&& other) noexcept 
    : _head(std::exchange(other._head, nullptr)),
      _validStart(std::exchange(other._validStart, 0)),
      _validEnd(std::exchange(other._validEnd, 0)) {}
```

### 3. Head Unlinking Bug in `allocate`
When a free block is completely exhausted (`curr->_size == 0`), you unlink it from the doubly-linked list. However, you forgot to update `_head` if the unlinked node is the first node.
```cpp
if (curr->_size == 0) {
    if (curr->_next != nullptr) curr->_next->_prev = curr->_prev;
    if (curr->_prev != nullptr) curr->_prev->_next = curr->_next;
    // BUG: What if curr == _head? _head is never updated!
}
```
* **Fix**: Add a check to update `_head`:
```cpp
if (curr == _head) _head = curr->_next;
```

### 4. Magic Numbers (`+ 1`)
You have arbitrary `+ 1` calculations when adjusting sizes and checking for adjacent blocks:
* `curr->_size -= nodeSize + 1;`
* `if (curr->_next != nullptr && curr->_next->_start == currEnd + 1)`
Memory addresses and sizes should be exact. Subtracting an extra byte causes you to lose 1 byte of memory on every allocation, and checking for `currEnd + 1` means contiguous blocks will fail to merge if they are exactly adjacent (which they should be).
* **Fix**: Remove the `+ 1`. Sizes and offsets should perfectly align (e.g., `currEnd = curr->_start + sizeof(FreeListNode) + curr->_size`).

### 5. Compaction Algorithm Efficiency
Your compaction loop in `deallocate` is $O(N^2)$ because it restarts from `_head` every time a merge occurs (`compact = false; break;`). 
* **Fix**: Since the list is sorted by address, you can merge adjacent nodes in a single $O(N)$ pass. Even better, since you know exactly where you just inserted the new free node, you only need to check its immediate `_prev` and `_next` nodes to see if they can be merged, making compaction $O(1)$.

### 6. Fragile Padding and Alignment
You hardcoded `std::byte _padding[16];` in `HeaderNode` to make it 32 bytes (matching `FreeListNode`). 
In `deallocate`, you do:
```cpp
dataAddr -= sizeof(HeaderNode);
```
This assumes the `_data` member in `Node<T>` is placed exactly after `HeaderNode`. If `T` has strict alignment requirements (e.g., a type requiring 64-byte alignment), the compiler will insert padding between `_header` and `_data` inside `Node<T>`, making your pointer arithmetic point to the wrong address.
* **Fix**: Use `offsetof(Node<T>, _data)` or `reinterpret_cast` the pointer back to `Node<T>*` before accessing the header, rather than relying on raw byte subtraction. Using a `union` for the Free/Header nodes is also a common pattern in allocators to avoid hardcoded padding.