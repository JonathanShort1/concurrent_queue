#include "lock_free_queue.h"

#include <string>
#include <iostream>

// -----------------------------------------------------------------------------
//                      LockFreeQueue PUBLIC FUNCTIONS
// -----------------------------------------------------------------------------


template <typename T>
LockFreeQueue<T>::LockFreeQueue()
: d_front(NULL)
, d_back(NULL)
, d_size(0)
{
}

template <typename T>
int LockFreeQueue<T>::size()
{
    return d_size;
}

template <typename T>
void LockFreeQueue<T>::printData()
{
}

template <typename T>
void LockFreeQueue<T>::enqueue(const T& payload)
{
    Node<T> *newNode = new Node<T>(payload);

    int empty = 0;
    if (std::atomic_compare_exchange_weak(&d_size, &empty, 1)) {
        d_front.store(newNode);
        d_back.store(newNode);
        return;
    }

    
    Node<T> *backTmp = d_back.load();
    backTmp->d_next = newNode;
    while(!std::atomic_compare_exchange_weak(&d_back, &backTmp, newNode)) {
        backTmp = d_back.load();
        backTmp->d_next = newNode;
    }

    ++d_size;
}

template <typename T>
T LockFreeQueue<T>::dequeue()
{
    T result;
    Node<T> *tmp;

    int empty = 0;
    if (std::atomic_compare_exchange_weak(&d_size, &empty, 0)) {
        return;
    }

    Node<T> *f = d_front.load();
    while(std::atomic_compare_exchange_weak(&d_front, &f, &f->d_next));
    result = f->d_value;
    d_size.store(d_size--);
    
    return result;
}

// Types to allow
template LockFreeQueue<int>::LockFreeQueue();
template int LockFreeQueue<int>::size();
template void LockFreeQueue<int>::printData();
template void LockFreeQueue<int>::enqueue(const int& payload);