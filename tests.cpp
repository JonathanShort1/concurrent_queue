#include "tests.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#define NUM_OPERATIONS 5
#define NUM_THREADS 10

template <typename T = int>
void producer(CQueue<T> *queue, int id, int numEnqueues)
{
    for (int i = 0; i < numEnqueues; ++i) {
        queue->enqueue(i + id);
        std::this_thread::sleep_for(std::chrono::milliseconds(id / 2));
    }
}

template <typename T = int>
void consumer(CQueue<T> *queue, int id,  int numEnqueues)
{
    for (int i = 0; i < numEnqueues; ++i) {
        queue->dequeue();
        std::this_thread::sleep_for(std::chrono::milliseconds(id / 2));
    }
}


// -----------------------------------------------------------------------------
//                      TESTS
// -----------------------------------------------------------------------------

template <typename T>
void test_enqueue_single_thread(CQueue<T>* queue)
{
    assert(queue->size() == 0);

    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        queue->enqueue(i);
    }
    assert(queue->size() == NUM_OPERATIONS);
}

template <typename T>
void test_dequeue_single_thread(CQueue<T>* queue)
{
    assert(queue->size() == 0);
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        queue->enqueue(i);
    }
    assert(queue->size() == NUM_OPERATIONS);

    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        queue->dequeue();
    }

    assert(queue->size() == 0);
}

template <typename T>
void test_enqueue_two_threads(CQueue<T>* queue)
{
    assert(queue->size() == 0);
    std::thread thread_arr[2];

    // enqueue
    for (int i = 0; i < 2; ++i) {
        thread_arr[i] = std::thread(producer<int>, queue, (i * 10), NUM_OPERATIONS);
    }
    for (int i = 0; i < 2; ++i) {
        thread_arr[i].join();
    }
    assert(queue->size() == (2 * NUM_OPERATIONS));
}

template <typename T>
void test_dequeue_two_threads(CQueue<T>* queue)
{
    assert(queue->size() == 0);
    std::thread thread_arr[2];

    // enqueue
    for (int i = 0; i < 2 * NUM_OPERATIONS; ++i) {
        queue->enqueue(i);
    }
    assert(queue->size() == (2 * NUM_OPERATIONS));

    // dequeue
    for (int i = 0; i < 2; ++i) {
        thread_arr[i] = std::thread(consumer<int>, queue, (i * 10), NUM_OPERATIONS);
    }
    for (int i = 0; i < 2; ++i) {
        thread_arr[i].join();
    }
    assert(queue->size() == 0);
}

template <typename T>
void test_enqueue_multi_threads(CQueue<T>* queue)
{
    assert(queue->size() == 0);
    std::thread thread_arr[NUM_THREADS];

    // enqueue
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_arr[i] = std::thread(producer<int>, queue, (i * 10), NUM_OPERATIONS);
    }
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_arr[i].join();
    }
    assert(queue->size() == (NUM_THREADS * NUM_OPERATIONS));
}

template <typename T>
void test_dequeue_multi_threads(CQueue<T>* queue)
{
    assert(queue->size() == 0);
    std::thread thread_arr[NUM_THREADS];

    // enqueue
    for (int i = 0; i < NUM_THREADS * NUM_OPERATIONS; ++i) {
       queue->enqueue(i);
    }
    assert(queue->size() == (NUM_THREADS * NUM_OPERATIONS));

    // dequeue
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_arr[i] = std::thread(consumer<int>, queue, (i * 10), NUM_OPERATIONS);
    }
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_arr[i].join();
    }
    assert(queue->size() == 0);
}

template <typename T>
void test_dequeue_empty_locking(LockingQueue<T>* queue)
{
    assert(queue->size() == 0);
    queue->dequeue();
    assert(queue->size() == 0);
}

template <typename T>
void test_dequeue_then_enqueue_once(LockFreeQueue<T>* queue)
{
    assert(queue->size() == 0);

    std::thread thread_arr[2];
    thread_arr[0] = std::thread(consumer<int>, queue, 10, 1);
    thread_arr[1] = std::thread(producer<int>, queue, 10, 1);
    thread_arr[0].join();
    thread_arr[1].join();
    assert(queue->size() == 1);
}

template <typename T>
void test_dequeue_then_enqueue_multi(LockFreeQueue<T>* queue)
{
    assert(queue->size() == 0);
    std::thread thread_arr[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i += 2) {
        thread_arr[i] = std::thread(consumer<int>, queue, (i * 10), NUM_OPERATIONS);
        thread_arr[i + 1] = std::thread(producer<int>, queue, (i * 10), NUM_OPERATIONS);
    }
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_arr[i].join();
    }
    // Cannot determinstically guarantee the size of queue
    // more showing that the queue doesn't live lock
}

template <typename T>
void run_test(void (*t)(CQueue<T> *), std::string testName)
{
    std::cout << "Runnning test: " << testName << " ";
    LockingQueue<int> *lq = new LockingQueue<int>();
    (*t)(lq);
    std::cout << " PASS for locking queue, ";

    LockFreeQueue<int> *fq = new LockFreeQueue<int>();
    (*t)(fq);
    std::cout << "PASS for lock free queue" << std::endl;
}

void test_main()
{
    std::cout << "Running tests" << std::endl;
    run_test<int>(&test_enqueue_single_thread, "test_enqueue_single_thread");
    run_test<int>(&test_dequeue_single_thread, "test_dequeue_single_thread");
    run_test<int>(&test_enqueue_two_threads, "test_enqueue_two_threads");
    run_test<int>(&test_dequeue_two_threads, "test_dequeue_two_threads");
    run_test<int>(&test_enqueue_multi_threads, "test_enqueue_multi_threads");
    run_test<int>(&test_dequeue_multi_threads, "test_dequeue_multi_threads");
    
    std::cout << "Runnning test: " << "test_dequeue_empty_locking" << " ";
    LockingQueue<int> *lq = new LockingQueue<int>();
    test_dequeue_empty_locking(lq);
    std::cout << "PASS" << std::endl;

    std::cout << "Runnning test: " << "test_dequeue_then_enqueue_once" << " ";
    LockFreeQueue<int> *fq = new LockFreeQueue<int>();
    test_dequeue_then_enqueue_once(fq);
    std::cout << "PASS" << std::endl;

    std::cout << "Runnning test: " << "test_dequeue_then_enqueue_multi" << " ";
    LockFreeQueue<int> *fq1 = new LockFreeQueue<int>();
    test_dequeue_then_enqueue_multi(fq1);
    std::cout << "PASS" << std::endl;
}

template void run_test(void (*f)(CQueue<int> *q), std::string testName);