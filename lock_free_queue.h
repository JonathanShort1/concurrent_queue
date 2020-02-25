#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H

#include "node.h"
#include <atomic>

template <typename T>
class LockFreeQueue {
    private:
        std::atomic<Node<T> *> d_front;
        std::atomic<Node<T> *> d_back;
        std::atomic<int>    d_size;

    public:
        // CREATORS
        LockFreeQueue();

        // ACCESSORS
        int size();
        void printData();
        
        // MODIFIERS
        void enqueue(const T& payload);
        T dequeue();
};

#endif