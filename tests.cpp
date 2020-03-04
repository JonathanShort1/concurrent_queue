#include "tests.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

#include "locking_queue.h"
#include "lock_free_queue.h"

#define NUM_OPERATIONS 5
#define NUM_THREADS 10

// -----------------------------------------------------------------------------
//                      THREAD WORKERS
// -----------------------------------------------------------------------------

/**
 * These functions are spawned by indiviual threads to operate on the given
 * 'queue'. 'id' is used to determine how long the thread should sleep for in
 * each iteration. 'numEnqueues' & 'numDequeues' determine the number of
 * operations the thread should perform on the queue.
 */
template <typename T>
void producer(CQueue<T> *queue, int id, int numEnqueues)
{
    for (int i = 0; i < numEnqueues; ++i) {
        queue->enqueue(i + id);
        std::this_thread::sleep_for(std::chrono::milliseconds(id / 2));
    }
}

template <typename T>
void consumer(CQueue<T> *queue, int id,  int numDequeues)
{
    for (int i = 0; i < numDequeues; ++i) {
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
void test_dequeue_empty_locking(CQueue<T>* queue)
{
    assert(queue->size() == 0);
    queue->dequeue();
    assert(queue->size() == 0);
}

template <typename T>
void test_dequeue_then_enqueue_once(CQueue<T>* queue)
{
    assert(queue->size() == 0);

    std::thread thread_arr[2];
    thread_arr[0] = std::thread(consumer<int>, queue, 10, 1);
    thread_arr[0].join();
    thread_arr[1] = std::thread(producer<int>, queue, 10, 1);
    thread_arr[1].join();

    assert(queue->size() == 1);
}

template <typename T>
void test_dequeue_then_enqueue_multi(CQueue<T>* queue)
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
    std::cout << "Runnning test: " << testName << std::endl << "\t(locking queue) ";
    LockingQueue<int> *lq = new LockingQueue<int>();
    (*t)(lq);
    std::cout << " PASS\n\t(lock free queue) ";

    LockFreeQueue<int> *fq = new LockFreeQueue<int>();
    (*t)(fq);
    std::cout << "PASS" << std::endl;
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
    run_test<int>(&test_dequeue_empty_locking, "test_dequeue_empty_locking");
    run_test<int>(&test_dequeue_then_enqueue_once, "test_dequeue_then_enqueue_once");
    run_test<int>(&test_dequeue_then_enqueue_multi, "test_dequeue_then_enqueue_multi");
    std::cout << "All passed!" << std::endl;
}

/* Types accepted by 'run_test'*/
template void run_test(void (*f)(CQueue<int> *q), std::string testName);