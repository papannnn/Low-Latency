#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <utility>

struct HeaderNode {
    size_t _start;
    size_t _size;

    HeaderNode(size_t start, size_t size): _start{start}, _size{size} {}
};

template <typename T>
struct Node {
    HeaderNode _header;
    T _data;

    template <typename... Args>
    Node(size_t start, size_t size, Args... args): _header{start, size}, _data{args...} {}
};

struct FreeListNode {
    size_t _start;
    size_t _size;
    FreeListNode* _next;
    FreeListNode* _prev;

    FreeListNode(size_t start, size_t size): _start{start}, _size{size}, _next{nullptr}, _prev{nullptr} {}
};

class FreeList {
private:
    FreeListNode* _head = nullptr;
public:
    FreeList(size_t capacity) {
        FreeListNode *head = reinterpret_cast<FreeListNode*>(malloc(capacity));
        head->_start = 0;
        head->_next = nullptr;
        head->_prev = nullptr;

        capacity -= sizeof(FreeListNode);

        head->_size = capacity;
        _head = head;
    }

    FreeList(FreeList&& freeList): _head(std::exchange(freeList._head, nullptr)) { }

    FreeList& operator=(FreeList&& freeList) {
        if (this != &freeList) {
            this->_head = freeList._head;
            freeList._head = nullptr;
        }
        return *this;
    }

    FreeList(FreeList& freeList) = delete;
    FreeList& operator=(FreeList& freeList) = delete;
    
    template<typename T, typename... Args>
    T* allocate(Args... args) {
        FreeListNode* curr = _head;
        while (curr != nullptr) {
            std::uintptr_t addrNode = reinterpret_cast<std::uintptr_t>(curr);
            std::uintptr_t addrAvail = addrNode + sizeof(FreeListNode);
            std::uintptr_t addrPlace = (addrAvail + curr->_size) - sizeof(Node<T>);
            size_t padding = addrPlace % alignof(Node<T>);
            size_t dataSize = padding + sizeof(Node<T>);

            if (curr->_size < dataSize) {
                curr = curr->_next;
                continue;
            }

            addrPlace = (addrAvail + curr->_size) - (sizeof(Node<T>) + padding);
            size_t valueStart = curr->_start + (addrPlace - addrNode);
            Node<T>* dataAddrPlace = new (reinterpret_cast<Node<T>*>(addrPlace)) Node<T>(valueStart, dataSize, args...);
            curr->_size -= dataSize;

            if (curr->_size == 0) {
                //
            }

            return &(dataAddrPlace->_data);
        }

        return nullptr;
    }

    void debug() {
        FreeListNode* curr = _head;
        while (curr != nullptr) {
            std::cout << "=====" << std::endl;
            std::cout << "Start: " << curr->_start << std::endl;
            std::cout << "Size: " << curr->_size << std::endl;
            curr = curr->_next;
        }
    }
};