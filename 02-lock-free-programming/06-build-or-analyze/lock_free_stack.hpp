#include <atomic>
#include <utility>

template <typename T>
struct Node {
    T data;
    Node *next;

    Node(T ele): data{std::move(ele)} {}
};

template <typename T>
class ConcurrentStack {
private:

    std::atomic<Node<T>*> _topPtr = nullptr;

public:
    
    void push(T data) {
        Node<T> *curr = new Node<T>(std::move(data));

        do {
            curr->next = _topPtr.load(std::memory_order_acquire);
        } while (!_topPtr.compare_exchange_weak(
            curr->next, 
            curr, std::memory_order_acq_rel, 
            std::memory_order_acquire)
        );
    }

    T top() {
        return _topPtr.load(std::memory_order_acquire)->data;
    }
};