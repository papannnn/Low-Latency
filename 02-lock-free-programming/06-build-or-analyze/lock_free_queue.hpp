#include <atomic>
#include <optional>

template <typename T>
struct Node {
    T _data;
    std::atomic<Node<T>*> _next;

    Node(): _data{}, _next{nullptr} {}
    Node(T data): _data{data}, _next{nullptr} {} 
};

template <typename T>
class LockFreeQueue {
private:
    std::atomic<Node<T>*> _head;
    std::atomic<Node<T>*> _tail;
public:
    LockFreeQueue() {
        Node<T>* node = new Node<T>();
        _head.store(node, std::memory_order_relaxed);
        _tail.store(node, std::memory_order_relaxed);
    }

    void push(T data) {
        Node<T>* node = new Node<T>(data);
        Node<T>* oldTail;
        while (true) {
            oldTail = _tail.load(std::memory_order_acquire);
            Node<T>* expected = nullptr;
            bool success = oldTail->_next.compare_exchange_weak(
                expected,
                node,
                std::memory_order_acq_rel,
                std::memory_order_acquire
            );
            
            if (success) {
                break;
            }
        }
        _tail.compare_exchange_weak(
            oldTail,
            oldTail->_next.load(std::memory_order_acquire),
            std::memory_order_acq_rel,
            std::memory_order_acquire
        );
    }

    std::optional<T> pop() {
        while (true) {
            Node<T>* currHead = _head.load(std::memory_order_acquire);
            Node<T>* currTail = _tail.load(std::memory_order_acquire);
            Node<T>* currNext = currHead->_next.load(std::memory_order_acquire);

            if (currHead == currTail && currNext == nullptr) {
                return std::nullopt;
            }

            bool successRemoveHead = _head.compare_exchange_weak(
                currHead,
                currNext,
                std::memory_order_acq_rel,
                std::memory_order_acquire
            );
            if (successRemoveHead) {
                auto retVal = std::optional<T>(std::move(currNext->_data));
                delete currHead;
                return retVal;
            }
        }
    }
};