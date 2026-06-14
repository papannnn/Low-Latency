# `std::deque`

## How element are stored

Deque actually store it's element inside a `block`. `block` is a static sized array.

Deque not only having one `block`, deque has more than one `block`. It's stored in an array called `map`. `map` is an array that stores pointer of `block`.

```text
                 _M_map
        +------+------+------+------+------+------+
        | null | node | node | node | node | null |
        +------+------+------+------+------+------+
                 |      |      |      |
                 v      v      v      v
              +-----+ +-----+ +-----+ +-----+
              | blk | | blk | | blk | | blk |
              +-----+ +-----+ +-----+ +-----+
                ^                         ^
                |                         |
             _M_start                 _M_finish
```

## When block are full

Assuming you're doing `push_back()` when the current `block` are full.

```text
push_back when the last buffer is full:

map before:  [ node ][ node ][ null ]
                         |
                         v
                    full buffer

map after:   [ node ][ node ][ node ]
                                |
                                v
                         new empty buffer
```

It will allocate memory first to create a static sized of block, and then put it inside the new block.

## Reason why use map and block

Assuming the deque purpose is for modifying the first / last element and random access.

That means we don't need to do unnecessary reallocating and moving all of the element when block is full, we just need to allocate new block to the next of current block.

Having an array of `block` pointer also improve the performance, if the `map` is full and we need to allocate 2x the size of the current `map`. We just need to move the pointer of the `block`, not the whole element.

Accessing data sequentially also good in CPU Caching since the element is contiguous in the block. The data pointing in the map is not contiguous, but at least the block is still contiguous.

## Source

https://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/a00536_source.html