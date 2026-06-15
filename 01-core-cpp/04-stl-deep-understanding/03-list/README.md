# `std::list`

## forward_list

Let's start from simple one, `forward_list`

```c++
template<typename T>
struct forward_list
{
    forward_list_node<T>* head;
};

template<typename T>
struct forward_list_node
{
    forward_list_node<T>* next;
    T value;
};
```

It's basically a linked list.



- Doubly linked list structure
- Node allocation
- Iterator stability
- Splice operations
- Cache locality tradeoffs

## Source

https://devblogs.microsoft.com/oldnewthing/20230804-00/?p=108547