#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <fstream>

#include "cqueue.h"

void performance_main();

template <typename T>
void performance_test(std::ofstream& outFile, CQueue<T> *queue, int flag);

#endif