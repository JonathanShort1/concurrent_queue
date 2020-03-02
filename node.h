#ifndef NODE_H
#define NODE_H

#include <atomic>

template<class T>
struct Node {
    T     d_value;
    std::atomic<Node<T> *>d_next;
    Node(const T& data) : d_value(data), d_next(nullptr) {}
};

#endif