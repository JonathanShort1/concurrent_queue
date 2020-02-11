#ifndef LOCKING_QUEUE_H
#define LOCKING_QUEUE_H

#include <mutex>
#include <queue>

template <typename T>
class LockingQueue {
    private:
        std::queue<T> d_data;
        std::mutex d_lock;

    public:
        // CREATORS
        LockingQueue() = default;

        // ACCESSORS
        int size();
        void printData();
        
        // MODIFIERS
        void enqueue(T payload);
        T dequeue();
};

#endif