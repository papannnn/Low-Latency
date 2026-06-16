# `std::set` / `std::multiset`

## Red black tree

Basically `set`, `multiset`, `multimap`, `map` are using Red Black Tree

```c++
struct tree
{
    node_base header; // or "node_base* header;"
    size_t size;
};

struct node_base
{
    node_base* parent;
    node_base* left;
    node_base* right;
};

struct node : node_base
{
    bool color; // red or black
    payload data;
};
```

The representation looks like this
```
                     +-----------+
                     |  HEADER   |
                     +-----------+
                     | parent=20 |
                     | left=5    |
                     | right=40  |
                     +-----------+
                           |
                           v

                           20
                         /    \
                       10      30
                      /  \       \
                     5   15      40
```

The reason it's also tracking the `left` and `right` for the header because iterator. Assume you want to do `begin()`. We need to know the smallest key in the Red Black Tree in constant time, we have `left` header node to do that.

## Time complextiy

| Operation    | Complexity     |
| ------------ | -------------- |
| `find()`     | O(log n)       |
| `insert()`   | O(log n)       |
| `erase(key)` | O(log n)       |
| `begin()`    | O(1)           |
| `end()`      | O(1)           |
| `++iterator` | amortized O(1) |
| `--iterator` | amortized O(1) |

## Source

https://devblogs.microsoft.com/oldnewthing/20230807-00/?p=108562