

#include <iostream>
#include <thread>
#include <chrono>

#include "locking_queue.h"
#include "lock_free_queue.h"

using namespace std;

void LockFunction(LockingQueue<int> *lq, int id)
{
    for (int i = 0; i < 5; ++i) {
        lq->enqueue(i + id);
        this_thread::sleep_for(chrono::milliseconds(id / 2));
    }
}

void freeFunc(LockFreeQueue<int> *fq, int id)
{
    for (int i = 0; i < 5; ++i) {
        fq->enqueue(i + id);
        this_thread::sleep_for(chrono::milliseconds(id / 2));
    }
}

int main()
{
    std::thread thread_arr[5];
    LockingQueue<int> *lq = new LockingQueue<int>();
    LockFreeQueue<int> *fq = new LockFreeQueue<int>();

    for (int i = 0; i < 5; ++i) {
        thread_arr[i] = std::thread(freeFunc, fq, (i * 10));
    }

    for (int i = 0; i < 5; ++i) {
        thread_arr[i].join();
    }

    cout << "size: " << fq->size() << endl;
}