#include <atomic>
#include <optional>
#include <utility>
#include <vector>

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
    
    TaggedPtr() = default;
    TaggedPtr(Node<T>* ptr, size_t tag): _ptr{ptr}, _tag{tag} {}
};

template <typename T>
class ConcurrentStack {
private:

    std::atomic<TaggedPtr<T>> _topPtr{};
    std::vector<Node<T>*> _retireList;

public:
    
    void push(T param) {
        Node<T> *data = new Node<T>(param);
        TaggedPtr<T> oldHead = _topPtr.load(std::memory_order_relaxed);
        TaggedPtr<T> newHead;
        do {
            data->next = oldHead._ptr;
            newHead = { data, oldHead._tag + 1 };
        } while (!_topPtr.compare_exchange_weak(oldHead, newHead, std::memory_order_acq_rel, std::memory_order_acquire));
    }

    std::optional<T> pop() {
        TaggedPtr<T> oldHead = _topPtr.load(std::memory_order_acquire);
        while (oldHead._ptr != nullptr) {
            TaggedPtr<T> newHead = { oldHead._ptr->next, oldHead._tag + 1 };
            if (_topPtr.compare_exchange_weak(oldHead, newHead, std::memory_order_acq_rel, std::memory_order_acquire)) {
                T val = oldHead._ptr->data;
                _retireList.push_back(oldHead._ptr);
                return val;
            }
        }
        return std::nullopt;
    }

    ~ConcurrentStack() {
        TaggedPtr<T> taggedPtr = _topPtr.load(std::memory_order_relaxed);
        Node<T>* curr = taggedPtr._ptr;
        while (curr != nullptr) {
            Node<T>* ptr = curr;
            curr = curr->next;
            delete ptr;
        }

        for (Node<T>* _ptr : _retireList) {
            delete _ptr;
        }
    }
};