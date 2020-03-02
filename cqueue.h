#ifndef CQUEUE_H
#define CQUEUE_H

template <typename T>
class CQueue {
    public:
        // ACCESSORS
        virtual int size() = 0;

        // MODIFIERS
        virtual void enqueue(const T& payload) = 0;
        virtual T dequeue() = 0;
};

#endif