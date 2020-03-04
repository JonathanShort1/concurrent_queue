/**
 * This header defines the functionality supplied by the performance suite.
 * 'performance_main(int) is the entry point into this functionality.
 * 'performance_test(...) runs the actual performance test on both types of
 * queue.
 */

#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <fstream>

#include "cqueue.h"

/**
 * This function is the entry point into the perormance suite.  It calls
 * 'performance_test()' twice.  Once for each implementation of a
 * concurrent queue. 
 */
void performance_main(int numCores);


/**
 * This function runs the performance test on the provided implementation of a
 * concurrent queue 'queue'.  The number of cores defines the number of threads
 * created by this test.  'flag' is used to distinguish the queue type in the 
 * csv file 'outFile'.  Locking queue = 0, lock-free queue = 1.
 */
template <typename T>
void performance_test(std::ofstream& outFile,
                      int numCores,
                      CQueue<T> *queue,
                      int flag);

#endif