#ifndef LOCKING_QUEUE_H
#define LOCKING_QUEUE_H

#include "cqueue.h"
#include "node.h"
#include <mutex>

template <typename T>
class LockingQueue : public CQueue<T>{
    private:
        Node<T>       *d_front;
        Node<T>       *d_back;
        std::mutex  d_lock;
        int         d_size;

    public:
        // CREATORS
        LockingQueue();

        // ACCESSORS
        int size();

        // MODIFIERS
        void enqueue(const T& payload);
        T dequeue();
};

#endif