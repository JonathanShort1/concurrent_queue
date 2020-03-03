#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <fstream>

#include "cqueue.h"

void performance_main(int numCores);

template <typename T>
void performance_test(std::ofstream& outFile, int numCores, CQueue<T> *queue, int flag);

#endif