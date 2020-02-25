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
void LockingQueue<T>::printData()
{
    d_lock.lock();
    Node<T> *curr = d_front;
    while (curr != NULL) {
        std::cout << curr->d_value << " ";
        curr = curr->d_next;
    }
    std::cout << std::endl;
    d_lock.unlock();
}

template <typename T>
void LockingQueue<T>::enqueue(const T& payload)
{
    d_lock.lock();

    Node<T> *new_node = new Node<T>(payload);
    if (d_front == NULL && d_back == NULL) {
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
        // would free tmp but think destructor resolves this memory leak
        delete tmp;
        --d_size;
    }
    d_lock.unlock();
    return result;
}

// Types to allow
template LockingQueue<int>::LockingQueue();
template int LockingQueue<int>::size();
template void LockingQueue<int>::printData();
template void LockingQueue<int>::enqueue(const int& payload);