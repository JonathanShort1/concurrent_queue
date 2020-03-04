#include "locking_queue.h"

#include <string>
#include <iostream>

// -----------------------------------------------------------------------------
//                      LockingQueue PUBLIC FUNCTIONS
// -----------------------------------------------------------------------------

// initialise all data members to NULL
template <typename T>
LockingQueue<T>::LockingQueue()
: d_front(nullptr)
, d_back(nullptr)
, d_size(0)
{
}

// acquire the lock before accessing the size data member
template <typename T>
int LockingQueue<T>::size()
{
    int size;
    d_lock.lock();
    size = d_size;
    d_lock.unlock();
    return size;
}

// acquire the lock and push the new data to the end of the queue
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

// acquire the lock, check if the queue is empty.  If not then remove data from
// the front of the queue. 
template <typename T>
T LockingQueue<T>::dequeue()
{
    T result;
    d_lock.lock();

    if(d_size > 0) {
        result = d_front->d_value;
        d_front = d_front->d_next;
        --d_size;
    } else {
        result = false;
    }
    
    d_lock.unlock();
    return result;
}

/* Types to allow */
template LockingQueue<int>::LockingQueue();
template int LockingQueue<int>::size();
template void LockingQueue<int>::enqueue(const int& payload);
template int LockingQueue<int>::dequeue();