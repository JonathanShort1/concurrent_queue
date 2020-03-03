#include "performance.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "locking_queue.h"
#include "lock_free_queue.h"

#define BUSY_WAIT 400
#define NUM_OPERATIONS 1000000

template <typename T>
void worker(CQueue<T> *queue, int divisor)
{
    int j = 0;
    int k;
    for (int i = 0; i < NUM_OPERATIONS / divisor; ++i) {
        // enqueue
        queue->enqueue(1);
        // do other work
        j = 0;
        while (j < BUSY_WAIT) { ++j; }
        // dequeue
        queue->dequeue();
        // do other work
        j = 0;
        while (j < BUSY_WAIT) { ++j; }
    }
}

template <typename T>
void performance_test(std::ofstream& outFile,
                      int numCores,
                      CQueue<T> *queue,
                      int flag)
{
    std::thread thread_arr[numCores];

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < numCores; ++i) {
        thread_arr[i] = std::thread(worker<int>, queue, numCores);
    }

    // join threads
    for (int i = 0; i < numCores; ++i) {
        thread_arr[i].join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << duration << std::endl;
    outFile << flag << "," << numCores << "," << duration << std::endl;
}


void performance_main(int numCores)
{
    std::cout << "Running performance experiments" << std::endl;
    LockingQueue<int> *lq = new LockingQueue<int>();
    LockFreeQueue<int> *fq = new LockFreeQueue<int>();
    std::ofstream outFile;

    outFile.open("performance.csv", std::ios_base::app);

    performance_test(outFile, numCores, lq, 0);
    performance_test(outFile, numCores, fq, 1);
}