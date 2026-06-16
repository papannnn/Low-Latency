# `std::map` / `std::unordered_map`

## How element stored

It's basically a hashtable.

```c++
struct hashtable
{
    using hint = std::list<payload>::iterator;

    std::list<payload> list;
    std::vector<hint> buckets;
};
```

Assuming currently we have 6 bucket.

```
+-------------------------+
| Bucket Array            |
+-------------------------+
| [0] --> nullptr         |
| [1] --> Node -> Node    |
| [2] --> nullptr         |
| [3] --> Node            |
| [4] --> Node -> Node    |
| [5] --> nullptr         |
+-------------------------+
```

You access the bucket by doing modulo on the key

```
bucket_count = 6

hash(10) % 6 = 4
hash(22) % 6 = 4
hash(15) % 6 = 3
hash(31) % 6 = 1
```

```
Bucket Array
============

[0] --> nullptr

[1] --> +----------------+
         | key=31 val=D  |
         | next=nullptr  |
         +----------------+

[2] --> nullptr

[3] --> +----------------+
         | key=15 val=C  |
         | next=nullptr  |
         +----------------+

[4] --> +----------------+
         | key=22 val=B  |
         | next ---------+----+
         +----------------+    |
                               v
                      +----------------+
                      | key=10 val=A  |
                      | next=nullptr  |
                      +----------------+

[5] --> nullptr
```

Inside the bucket, it's basically a linked list. If there's more than 1 key shares the same bucket, it will append the element into the tail of the linked list.

## Source

https://devblogs.microsoft.com/oldnewthing/20230808-00/?p=108572