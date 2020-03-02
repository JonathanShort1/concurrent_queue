#include "lock_free_queue.h"

#include <string>
#include <iostream>

// -----------------------------------------------------------------------------
//                      LockFreeQueue PUBLIC FUNCTIONS
// -----------------------------------------------------------------------------


template <typename T>
LockFreeQueue<T>::LockFreeQueue()
: d_size(0)
{
    Node<T> *dummy = new Node<T>(0);
    d_front = d_back = dummy;
}

template <typename T>
int LockFreeQueue<T>::size()
{
    return d_size;
}

template <typename T>
void LockFreeQueue<T>::enqueue(const T& payload)
{
    Node<T> *newNode = new Node<T>(payload);

    Node<T> *tail;
    Node<T> *next;
    while(1) {
        tail = d_back.load();
        next = tail->d_next;
        if (tail == d_back) {
            if (next == NULL) {
                if (std::atomic_compare_exchange_weak<Node<T> *>(&tail->d_next, &next, newNode)) {
                    ++d_size;
                    break;
                }
            } else {
                std::atomic_compare_exchange_weak<Node<T> *>(&d_back, &tail, next);
            }
        }
    }

    std::atomic_compare_exchange_weak(&d_back, &tail, newNode);
}

template <typename T>
T LockFreeQueue<T>::dequeue()
{
    T result;

    while(1) {
        Node<T> *head = d_front;
        Node<T> *tail = d_back;
        Node<T>* next = head->d_next;

        if (head == d_front) {
            if (head == tail) {
                if (next == NULL) {
                    // QUEUE IS EMPTY
                    return false;
                }
                std::atomic_compare_exchange_weak(&d_back, &tail, next);
            } else {
                result = next->d_value;
                if (std::atomic_compare_exchange_weak(&d_front, &head, next)) {
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