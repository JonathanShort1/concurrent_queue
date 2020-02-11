#ifndef CQUEUE_H
#define CQUEUE_H

#include <queue>

template <typename T>
class CQueue {
    public:
        int           d_size;
        std::queue<T> d_q;

        // MODIFIERS
        virtual void enqueue(T payload) = 0;
        virtual T dequeue() = 0;

};

#endif