#include "locking_queue.h"

#include <string>
#include <iostream>

// -----------------------------------------------------------------------------
//                      LockingQueue PUBLIC FUNCTIONS
// -----------------------------------------------------------------------------


template <typename T>
LockingQueue<T>::LockingQueue()
: d_front(NULL)
, d_back(NULL)
, d_size(0)
{
}

template <typename T>
int LockingQueue<T>::size()
{
    return d_size;
}

template <typename T>
void LockingQueue<T>::enqueue(const T& payload)
{
    d_lock.lock();

    Node<T> *new_node = new Node<T>(payload);
    if (d_size == 0) {
        d_front = d_back = new_node;
    } else {
        d_back->d_next = new_node;
        d_back = new_node;
    }
    ++d_size;

    d_lock.unlock();
}

template <typename T>
T LockingQueue<T>::dequeue()
{
    T result;
    Node<T> *tmp;
    d_lock.lock();
    if(d_size > 0) {
        tmp = d_front;
        result = tmp->d_value;
        d_front = tmp->d_next;
        --d_size;
    }
    d_lock.unlock();
    return result;
}

// Types to allow
template LockingQueue<int>::LockingQueue();
template int LockingQueue<int>::size();
template void LockingQueue<int>::enqueue(const int& payload);
template int LockingQueue<int>::dequeue();