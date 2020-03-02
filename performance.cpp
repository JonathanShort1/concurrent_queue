#include "performance.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "locking_queue.h"
#include "lock_free_queue.h"

#define NUM_THREADS 10
#define NUM_OPERATIONS 100000

template <typename T>
void worker(CQueue<T> *queue, int divisor)
{
    for (int i = 0; i < NUM_OPERATIONS / divisor; ++i) {
        // enqueue
        queue->enqueue(1);
        // do other work
        std::this_thread::sleep_for(std::chrono::nanoseconds(10));
        // dequeue
        queue->dequeue();
        // do other work
        std::this_thread::sleep_for(std::chrono::nanoseconds(10));
    }
}

template <typename T>
void performance_test(std::ofstream& outFile, CQueue<T> *queue, int flag)
{
    std::thread thread_arr[NUM_THREADS][NUM_THREADS];

    for (int i = 1; i < NUM_THREADS; ++i) {
        // run test
        auto start = std::chrono::high_resolution_clock::now();

        for (int j = 0; j < i; ++j) {
            thread_arr[i - 1][j] = std::thread(worker<int>, queue, i);
        }

        // join threads
        for (int j = 0; j < i; ++j) {
            thread_arr[i - 1][j].join();
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        std::cout << duration << std::endl;
        outFile << flag << "," << i << "," << duration << std::endl;

    }
}


void performance_main()
{
    std::cout << "Running performance experiments" << std::endl;
    LockingQueue<int> *lq = new LockingQueue<int>();
    LockFreeQueue<int> *fq = new LockFreeQueue<int>();
    std::ofstream outFile;

    outFile.open("performance.csv");

    outFile << "QueueType,NumberOfThreads, AverageDuration" << std::endl;

    performance_test(outFile, lq, 0);
    performance_test(outFile, fq, 1);
}