#include <atomic>
#include <optional>
#include <utility>

template <typename T>
struct Node {
    T data;
    Node *next;

    Node(T ele): data{std::move(ele)}, next{nullptr} {}
};

template <typename T>
struct TaggedPtr {
    Node<T>* _ptr;
    size_t _tag;
    
    TaggedPtr() {}
    TaggedPtr(Node<T>* ptr): _ptr{ptr}, _tag{1} {}
};

template <typename T>
class ConcurrentStack {
private:

    std::atomic<TaggedPtr<Node<T>>> _topPtr{};

public:
    
    void push(T param) {
        Node<T> *data = new Node<T>(param);
        TaggedPtr<Node<T>> oldHead = _topPtr.load(std::memory_order_relaxed);
        TaggedPtr<Node<T>> newHead;
        do {
            data->next = oldHead._ptr;
            newHead = { data, oldHead._tag };
        } while (!_topPtr.compare_exchange_weak(oldHead, newHead, std::memory_order_acq_rel, std::memory_order_acquire));
    }

    std::optional<T> pop() {
        TaggedPtr<Node<T>> oldHead = _topPtr.load(std::memory_order_acquire);
        while (oldHead._ptr != nullptr) {
            TaggedPtr<Node<T>> newHead = { oldHead._ptr->next, oldHead._tag + 1 };
            if (_topPtr.compare_exchange_weak(oldHead, newHead, std::memory_order_acq_rel, std::memory_order_acquire)) {
                T val = oldHead._ptr->data;
                return val;
            }

            oldHead = _topPtr.load(std::memory_order_acquire);
        }
        return std::nullopt;
    }
};