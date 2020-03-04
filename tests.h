/**
 * This header defines the interface for the units tests used to define the
 * desired behaviour of the concurrent queues.  Each test is ran on both types 
 * of queue using the 'run_test()' function.  'test_main()' is the entry point 
 * to the test suite. 
 */
#ifndef TESTS_H
#define TESTS_H

#include <string>

#include "cqueue.h"

/**
 * This function calls every unit test by calling run_test() and passing each
 * unit test as a parameter. 
 */
void test_main();

/**
 * This function creates one locking queue and one lock-free queue. It then
 * calls the unit test passed as function pointer 'f' and as string 'testName'.
 * If either test run fails the program will crash on the C-style assert.  If
 * both suceed then the two queues will be destructed once they leave scope
 * ensuring independence between any two tests.
 */
template <typename T>
void run_test(void (*f)(CQueue<T> *), std::string testName);

/**
 * Tests to ensure that each queue implementation operates as desired
 * sequentially.  First for enqueue() and second for dequeue().
 */
template <typename T>
void test_enqueue_single_thread(CQueue<T> *queue);

template <typename T>
void test_dequeue_single_thread(CQueue<T>* queue);

/**
 * Tests to ensure that the queue implementations have the correct semantics when
 * two threads compete on the same queues.  First for enqueue() and second for
 * dequeue().  A sequential enqueue() is used for the dequeue() test.
 */
template <typename T>
void test_enqueue_two_threads(CQueue<T>* queue);

template <typename T>
void test_dequeue_two_threads(CQueue<T>* queue);

/**
 * Tests to ensure that the queue implementations have the correct semantics
 * when multiple threads are competing to enqueue() and dequeue().  First for
 * enqueue() and second for dequeue().  A sequential enqueue() is used for the
 * dequeue() test.
 */
template <typename T>
void test_enqueue_multi_threads(CQueue<T>* queue);

template <typename T>
void test_dequeue_multi_threads(CQueue<T>* queue);

/**
 * Tests to ensure that the queue implementations have the correct semantics
 * when attempting to dequeue() an empty queue.  The supplied interface does not
 * state the behaviour of the dequeue() function.  Namely, whether it should
 * block while there is nothing to consume; orif an exception should be thrown;
 * or if an invlaid result should be returned.  The chosen behaviour is that
 * dequeue() is non-blocking when the queue is empty. This is first tested on an
 * empty queue, then to dequeue() explicitly before an enqueue() and finally to
 * interleave dequeue()'s and enqueue()'s starting with an initial dequeue().
 */
template <typename T>
void test_dequeue_empty_locking(CQueue<T>* queue);

template <typename T>
void test_dequeue_then_enqueue_once(CQueue<T>* queue);

template <typename T>
void test_dequeue_then_enqueue_multi(CQueue<T>* queue);

#endif