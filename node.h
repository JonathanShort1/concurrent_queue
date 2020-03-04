/**
 * This struct defines a node in a concurrent queue implementation.  The d_next
 * node is atomic so that it can be used in the C++11 atomic CAS functions.
 */

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