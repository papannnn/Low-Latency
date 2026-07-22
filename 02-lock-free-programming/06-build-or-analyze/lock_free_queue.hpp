#include <atomic>

template <typename T>
struct Node {
    T _data;
    Node* _next;
    Node* _prev;

    Node(T data): _data{data} {}
};

template <typename T>
struct TaggedPtr {
    Node<T>* _ptr;
    size_t _tag;

    TaggedPtr() = default;
    TaggedPtr(Node<T>* ptr, size_t tag): _ptr{ptr}, _tag{tag} {}
};

template <typename T>
class LockFreeQueue {
private:
    std::atomic<TaggedPtr<T>> _head{nullptr, 0};
    std::atomic<TaggedPtr<T>> _tail{nullptr, 0};
public:
    void push(T data) {
        Node<T>* currNode = new Node<T>(data);
        TaggedPtr<T> oldTail = _tail.load(std::memory_order_acquire);
        TaggedPtr<T> newTail;
        do {
            newTail = { currNode, oldTail._tag + 1 };
            currNode->_next = oldTail._ptr;
        } while (!_tail.compare_exchange_weak(oldTail, newTail, std::memory_order_acq_rel, std::memory_order_acquire));
        
        if (oldTail._ptr != nullptr) {
            oldTail._ptr->_prev = newTail._ptr;
        }
    }
};