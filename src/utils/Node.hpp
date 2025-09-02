#include <iostream>
#include <memory>

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;

template<typename T>
class Node {
public:
    shared_ptr<Node> next;
    weak_ptr<Node> prev;
    T val = {};

    explicit Node(T value) : val(value) {};

    static inline shared_ptr<Node> with_capacity(size_t n) {
        if (n == 0) {
            throw std::invalid_argument("n >= 1");
        }
        auto start = make_shared<Node<T>>(0);
        auto cur = start;
        for (size_t i = 1; i < n; ++i) {
            cur->next = make_shared<Node<T>>(i);
            cur->next->prev = cur;
            cur = cur->next;
        }
        cur->next = start;
        start->prev = cur;
        return start;
    }

    void detach() {
        auto p = prev.lock();
        if (next && p) {
            next->prev = p;
            p->next = next;
        }

        next.reset();
        prev.reset();
    }
};
