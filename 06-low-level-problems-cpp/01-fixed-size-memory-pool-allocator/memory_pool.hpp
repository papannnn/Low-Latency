#pragma once

#include <iostream>
#include <stdlib.h>

template <typename T>
struct Node {
    T _data;
    Node<T>* _prev;
    Node<T>* _next;

    Node(): _data{}, _prev{nullptr}, _next{nullptr} {}
    Node(T data): _data{data}, _prev{nullptr}, _next{nullptr} {}
};

template<typename T>
class MemoryPool {
private:
    Node<T>* _head = nullptr;
    void* _mem = nullptr;
public:
    MemoryPool(size_t allocated_memory) {
        _mem = malloc(allocated_memory);
        uintptr_t start = reinterpret_cast<uintptr_t>(_mem);
        uintptr_t end = start + allocated_memory;

        uintptr_t curr = start;
        size_t nodeSize = sizeof(Node<T>);
        while (curr < end) {
            Node<T>* data = reinterpret_cast<Node<T>*>(curr);
            if (_head == nullptr) {
                _head = data;
            } else {
                _head->_prev = data;
                _head = data;
            }
            curr += nodeSize;
        }
    }

    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    T* allocate(T val) {
        if (_head == nullptr) {
            std::cout << "Can't allocate more" << std::endl;
            return nullptr;
        }

        Node<T>* data = _head;
        _head = _head->_next;
        new (&(data->_data)) T(val);
        return &(data->_data);
    }

    void deallocate(T* ptr) {
        Node<T>* node = reinterpret_cast<Node<T>*>(ptr);
        // node->~_data();
        _head->_prev = node;
        _head = _head->_prev;
    }
};