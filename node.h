#ifndef NODE_H
#define NODE_H

template<class T>
struct Node
{
    T     d_value;
    Node *d_next;
    Node(const T& data) : d_value(data), d_next(nullptr) {}
};

#endif