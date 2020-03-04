#include "lock_free_queue.h"

#include <string>
#include <iostream>

// -----------------------------------------------------------------------------
//                      LockFreeQueue PUBLIC FUNCTIONS
// -----------------------------------------------------------------------------

// Initialise the Queue with one dummy node to change the empty condition from
// (d_front == NULL && d_back == NULL) to (d_front == d_back)
template <typename T>
LockFreeQueue<T>::LockFreeQueue()
: d_size(0)
{
    Node<T> *dummy = new Node<T>(0);
    d_front = d_back = dummy;
}

// atomically load the size into a tempary variable
template <typename T>
int LockFreeQueue<T>::size()
{
    int size = d_size.load(std::memory_order_seq_cst);
    return size;
}

/**
 * 
 */
template <typename T>
void LockFreeQueue<T>::enqueue(const T& payload)
{
    Node<T> *newNode = new Node<T>(payload);

    Node<T> *tail;
    Node<T> *next;
    while(1) {
        tail = d_back.load(std::memory_order_relaxed);
        next = tail->d_next;
        if (tail == d_back) {
            if (next == NULL) {
                if (std::atomic_compare_exchange_weak_explicit<Node<T> *>(
                                                &tail->d_next,
                                                &next,
                                                newNode,
                                                std::memory_order_relaxed,
                                                std::memory_order_relaxed)) {
                    ++d_size;
                    break;
                }
            } else {
                std::atomic_compare_exchange_weak_explicit<Node<T> *>(
                                                     &d_back,
                                                     &tail,
                                                     next,
                                                     std::memory_order_relaxed,
                                                     std::memory_order_relaxed);
            }
        }
    }

    std::atomic_compare_exchange_weak_explicit(&d_back,
                                               &tail,
                                               newNode,
                                               std::memory_order_relaxed,
                                               std::memory_order_relaxed);
}

template <typename T>
T LockFreeQueue<T>::dequeue()
{
    T result;

    while(1) {
        Node<T> *head = d_front.load(std::memory_order_relaxed);
        Node<T> *tail = d_back.load(std::memory_order_relaxed);
        Node<T>* next = head->d_next;

        if (head == d_front) {
            if (head == tail) {
                if (next == NULL) {
                    // QUEUE IS EMPTY
                    return false;
                }
                std::atomic_compare_exchange_weak_explicit(
                                                    &d_back,
                                                    &tail,
                                                    next,
                                                    std::memory_order_relaxed,
                                                    std::memory_order_relaxed);
            } else {
                result = next->d_value;
                if (std::atomic_compare_exchange_weak_explicit(
                                                &d_front,
                                                &head,
                                                next,
                                                std::memory_order_relaxed,
                                                std::memory_order_relaxed)) {
                    // successful dequeue
                    --d_size;
                    break;
                }
            }
        }
    }

    return result;
}

// Types to allow
template LockFreeQueue<int>::LockFreeQueue();
template int LockFreeQueue<int>::size();
template void LockFreeQueue<int>::enqueue(const int& payload);
template int LockFreeQueue<int>::dequeue();