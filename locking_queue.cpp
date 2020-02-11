#include "locking_queue.h"

#include <string>
#include <iostream>

// -----------------------------------------------------------------------------
//                      LockingQueue PUBLIC FUNCTIONS
// -----------------------------------------------------------------------------
template <typename T>
int LockingQueue<T>::size(){
    return d_data.size();
}

template <typename T>
void LockingQueue<T>::printData(){
    std::queue<T> q; // temp queue
    std::cout << "Data: ";
    T tmp;
    while(!d_data.empty()) {
        tmp = d_data.front();
        d_data.pop();
        std::cout << tmp << " ";
        q.push(tmp); // keep value
    }

    std::cout << std::endl;

    d_data.swap(q); // restore queue
}

template <typename T>
void LockingQueue<T>::enqueue(T payload)
{
    d_lock.lock();
    d_data.push(payload);
    d_lock.unlock();
}

template <typename T>
T LockingQueue<T>::dequeue()
{
    int result;
    d_lock.lock();
    result = d_data.front();
    d_data.pop();
    d_lock.unlock();
    return result;
}

// Types to allow
template int LockingQueue<int>::size();
template int LockingQueue<char>::size();
template int LockingQueue<std::string>::size();

template void LockingQueue<int>::printData();
template void LockingQueue<char>::printData();
template void LockingQueue<std::string>::printData();

template void LockingQueue<int>::enqueue(int payload);
template void LockingQueue<char>::enqueue(char payload);
template void LockingQueue<std::string>::enqueue(std::string payload);