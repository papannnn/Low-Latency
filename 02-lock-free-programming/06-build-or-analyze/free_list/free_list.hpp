#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <utility>

struct HeaderNode {
    size_t _start;
    size_t _size;
    std::byte _padding[16];

    HeaderNode(size_t start, size_t size): _start{start}, _size{size} {}
};

struct FreeListNode {
    size_t _start;
    size_t _size;
    FreeListNode* _next;
    FreeListNode* _prev;

    FreeListNode(size_t start, size_t size): _start{start}, _size{size}, _next{nullptr}, _prev{nullptr} {}
};

template <typename T>
struct Node {
    HeaderNode _header;
    T _data;

    template <typename... Args>
    Node(size_t start, size_t size, Args... args): _header{start, size}, _data{args...} {}
};

class FreeList {
private:
    FreeListNode* _head = nullptr;
    std::uintptr_t _validStart{};
    std::uintptr_t _validEnd{};
public:
    FreeList(size_t capacity) {
        FreeListNode *head = reinterpret_cast<FreeListNode*>(malloc(capacity));
        head->_start = 0;
        head->_next = nullptr;
        head->_prev = nullptr;

        _validStart = reinterpret_cast<uintptr_t>(head);
        _validEnd = _validStart + capacity;

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

    ~FreeList() {
        free(reinterpret_cast<void*>(_validStart));
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
            size_t nodeSize = padding + sizeof(Node<T>);
            if (curr->_size < padding + 1) {
                curr = curr->_next;
                continue;
            }

            addrPlace = (addrAvail + curr->_size) - (sizeof(Node<T>) + padding);
            size_t valueStart = curr->_start + (addrPlace - addrNode);
            Node<T>* dataAddrPlace = new (reinterpret_cast<Node<T>*>(addrPlace)) Node<T>(valueStart, nodeSize, args...);
            curr->_size -= nodeSize + 1;

            if (curr->_size == 0) {
                if (curr->_next != nullptr) {
                    curr->_next->_prev = curr->_prev;
                }

                if (curr->_prev != nullptr) {
                    curr->_prev->_next = curr->_next;
                }
            }

            return &(dataAddrPlace->_data);
        }

        return nullptr;
    }

    void deallocate(void* data) {
        std::uintptr_t dataAddr = reinterpret_cast<uintptr_t>(data);
        dataAddr -= sizeof(HeaderNode);
        if (dataAddr < _validStart || dataAddr > _validEnd) {
            return;
        }

        HeaderNode* dataNode = reinterpret_cast<HeaderNode*>(dataAddr);
        size_t start = dataNode->_start;
        size_t size = dataNode->_size - sizeof(HeaderNode);
        FreeListNode* freeListNode = new (dataNode) FreeListNode(start, size);

        if (_head == nullptr) {
            _head = freeListNode;
            return;
        }

        if (start < _head->_start) {
            _head->_prev = freeListNode;
            freeListNode->_next = _head;
            _head = freeListNode;
        } else {
            FreeListNode* curr = _head;
            FreeListNode* next = curr->_next;
            bool found = false;
            while (next != nullptr) {
                if (start < next->_start) {
                    freeListNode->_next = next;
                    freeListNode->_prev = curr;
                    curr->_next = freeListNode;
                    next->_prev = freeListNode;
                    found = true;
                    break;
                }
                curr = next;
                next = next->_next;
            }

            if (!found) {
                curr->_next = freeListNode;
                freeListNode->_prev = curr;
            }
        }

        bool compact = false;
        while (!compact) {
            compact = true;
            FreeListNode* curr = _head;
            while (curr != nullptr) {
                size_t currEnd = curr->_start + sizeof(FreeListNode) + curr->_size;
                if (curr->_next != nullptr && curr->_next->_start == currEnd + 1) {
                    curr->_size += curr->_next->_size + sizeof(FreeListNode) + 1;
                    if (curr->_next->_next != nullptr) {
                        curr->_next->_next->_prev = curr;
                    }

                    curr->_next = curr->_next->_next;
                    compact = false;
                    break;
                }
                curr = curr->_next;
            }
        }
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