/**
 * This header defines the public interface for a concurrent queue.
 */

#ifndef CQUEUE_H
#define CQUEUE_H

template <typename T>
class CQueue {
    public:
        // ACCESSORS
        /**
         * This function is thread-safe and returns the current size of the
         * queue. 
         */
        virtual int size() = 0;

        // MODIFIERS
        /**
         * This function adds an element of type 'T' to the queue(). If multiple
         * threads are concurrently enqueue'ing the order of enqueues is NOT
         * guaranteed.
         */
        virtual void enqueue(const T& payload) = 0;

        /**
         * This function removes an element from the queue. If the queue is
         * empty the boolean value 'false' is returned. This function does NOT
         * block if the queue is empty.
         */
        virtual T dequeue() = 0;
};

#endif