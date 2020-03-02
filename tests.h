#ifndef TESTS_H
#define TESTS_H

#include "cqueue.h"
#include "locking_queue.h"
#include "lock_free_queue.h"

void test_main();

template <typename T>
void run_test(void (*f)(CQueue<T> *));

template <typename T>
void test_enqueue_single_thread(CQueue<T> *queue);

template <typename T>
void test_dequeue_single_thread(CQueue<T>* queue);

template <typename T>
void test_enqueue_two_threads(CQueue<T>* queue);

template <typename T>
void test_dequeue_two_threads(CQueue<T>* queue);

template <typename T>
void test_enqueue_multi_threads(CQueue<T>* queue);

template <typename T>
void test_dequeue_multi_threads(CQueue<T>* queue);

template <typename T>
void test_dequeue_empty_locking(LockingQueue<T>* queue);

template <typename T>
void test_dequeue_then_enqueue_once(LockFreeQueue<T>* queue);

template <typename T>
void test_dequeue_then_enqueue_multi(LockFreeQueue<T>* queue);

#endif