# `std::vector`

## Interface

## Element Access

There's some of way to access element in vector

### `at`

At is accessing the array of the vector using bound checking, it will give the reference of the element if idx is valid, and throw `std::out_of_range` when fail.

Complexity is constant.

### `operator[]`

Same like `at`, but without bound checking.

Before C++26, when idx is out of range, it will causing UB.

Complexity is constant.

### `front`

If vector is empty, `front` will be UB.

It's basically same like `*c.begin()`

Complexity is constant

### `data`

Data basically just returning a pointer of the first index of the array.

Complexity is constant

## Capacity

### empty

Checking if the vector empty or not, 

`begin() == end()`

Complexity is constant

### size

Return the size of the array

Complexity is constant

### max_size

Return the maximum amount of size that vector can handle

Complexity is constant

### reserve

Increase the capacity of vector when reserve param is larger than the current one, otherwise, do nothing.

Complexity is linear with current size of vector.

### Capacity

Return the number of current capacity of the vector

Complexity is constant

### shrink_to_fit

It will change the capacity to the current size.

Complexity is linear with current size of vector.

## Modifier

### clear

Clear all element in the vector, after this, size will become zero.

A typical implementation usually

```c++
void clear() {
    for (size_t i = 0; i < size_; ++i) {
        data_[i].~T();
    }
    size_ = 0;
}
```

Complexity is linear with the current size vector

### insert

```c++
iterator insert( const_iterator pos, const T& value );

iterator insert( const_iterator pos, T&& value );

iterator insert( const_iterator pos,
                 size_type count, const T& value );

template< class InputIt >
iterator insert( const_iterator pos, InputIt first, InputIt last );

iterator insert( const_iterator pos, std::initializer_list<T> ilist );
```

Basically, you can insert in the middle of vector with insert, but complexity will be mostly linear.

### insert_range

Basically same like insert, but can put range of values

```c++
const auto rg = std::list{-1, -2, -3};

container.insert_range(pos, rg);
```

### emplace

Like insert, but use parameterized constructor.

### erase

Can insert in the middle of the vector

But the complexity kinda bad, it's  mostly linear

### push_back

Interface will look like this

```c++
void push_back( const T& value ); (1)

void push_back( T&& value ); (2)
```

Append the value to the end of container

1. New element is inserted by copying the value
2. New element is inserted by moving the value.

Complexity is amortized constant

Amortized constant because there's a scenario where the push_back happens when capacity is already full.

That means the vector need to allocated a new larger array first, moving all of that, then push_back.

### emplace_back

Almost same like push_back, but it's using parameterized constructor when insert on the back, that means no need to do copying, since it's constructed inplace.

Same complexity like push_back, because there's a chance of reallocate the array.

### append_range

Same like push_back, but pass the array.

Complexity depends on scenario, if array reallocation happen, it will be linear vector size + size of the param. If not, only size of the param.

### pop_back

If called when vector empty, it will UB.

If not, it will remove the last element

Complexity is constant

### resize

Interface

```c++
void resize( size_type count );

void resize( size_type count, const value_type& value );
```

Example code

```c++
#include <vector>
#include <iostream>

void print(auto rem, const std::vector<int>& c)
{
    for (std::cout << rem; const int el : c)
        std::cout << el << ' ';
    std::cout << '\n';
}

int main()
{
    std::vector<int> c = {1, 2, 3};
    print("The vector holds: ", c);

    c.resize(5);
    print("After resize up to 5: ", c);

    c.resize(2);
    print("After resize down to 2: ", c);

    c.resize(6, 4);
    print("After resize up to 6 (initializer = 4): ", c);
}
```

Output

```
The vector holds: 1 2 3
After resize up to 5: 1 2 3 0 0
After resize down to 2: 1 2
After resize up to 6 (initializer = 4): 1 2 4 4 4 4
```

### swap

Swap the array with another vector array

Complexity is constant

## Source

https://en.cppreference.com/cpp/container/vector