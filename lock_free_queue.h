/**
 * This header represents the implementation of a lock-free concurrent queue.
 * It inherits a pulbic interface from the class CQueue.
 */

#ifndef LOCK_FREE_QUEUE_H
#define LOCK_FREE_QUEUE_H

#include <atomic>
#include "cqueue.h"
#include "node.h"

template <typename T>
class LockFreeQueue : public CQueue<T> {
    private:
        std::atomic<Node<T> *> d_front;
        std::atomic<Node<T> *> d_back;
        std::atomic<int>    d_size;

    public:
        // CREATORS
        LockFreeQueue();

        // ACCESSORS
        int size();

        // MODIFIERS
        void enqueue(const T& payload);
        T dequeue();
};

#endif