

#include <iostream>
#include <thread>
#include <chrono>

#include "locking_queue.h"

using namespace std;

void function(LockingQueue<int> *lq, int id)
{
    int i = 0;
    while (i < 5) {
        lq->enqueue(i + id);
        ++i;
        this_thread::sleep_for(chrono::milliseconds(id / 2));
    }
}

int main()
{
    std::thread thread_arr[5];
    LockingQueue<int> *lq = new LockingQueue<int>();

    for (int i = 0; i < 5; ++i) {
        thread_arr[i] = std::thread(function, lq, (i * 10));
    }

    for (int i = 0; i < 5; ++i) {
        thread_arr[i].join();
    }

    cout << "size: " << lq->size() << endl;
    lq->printData();

    
}