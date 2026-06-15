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

```
head
 |
 v
+------+-------+    +------+-------+    +------+-------+
|  10  |   o---+--->|  20  |   o---+--->|  30  | nullptr
+------+-------+    +------+-------+    +------+-------+
```

Now, let's see the `list`

```c++
template<typename T>
struct list
{
    list_node_base<T> head; // or "list_node_base<T>* head;"
    size_t size;
};

template<typename T>
struct list_node_base
{
    list_node<T>* next;
    list_node<T>* prev;
};

template<typename T>
struct list_node : list_node_base<T>
{
    T value;
};
```

It's a doubly linked list basically

```
+----------+      +------+      +------+      +------+
| sentinel | <--> | 10   | <--> | 20   | <--> | 30   |
+----------+      +------+      +------+      +------+
      ^                                          |
      |__________________________________________|
```

Because it's a linked list, the element are not contiguous, and it's actually really bad for cache locality. 

## Source

https://devblogs.microsoft.com/oldnewthing/20230804-00/?p=108547