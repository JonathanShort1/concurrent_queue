#include <cstdlib>
#include <iostream>

#include "performance.h"
#include "tests.h"

using namespace std;

/**
 * This function is the main entry to the concurrent queue implementations.
 * This program takes one argument.
 * 
 * opt: 
 *      -1: Run the unit tests
 *  1 - 12: Run the performance tests with <opt> threads (if ran with command
 *          taskset then each thread can run on a separate core, see man
 *          taskset)
 */
int main(int argc, char *argv[])
{
    if (argc == 1) {
        cout << "Usage: opt == -1 for testing, opt == (1-12) for performance\n";
        cout << "taskset <number of cores in bitmask> ./main; ./main <number of cores in dec>\n";
        return -1;
    }

    int opt = std::strtol(argv[1], nullptr, 0);

    switch(opt) {
        case -1:
            test_main();
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            performance_main(opt);
            break;
        default:
            cout << "Usage: opt == -1 for testing, opt == (1-12) for performance\n";
            cout << "taskset <number of cores in bitmask> ./main; ./main <number of cores in dec>\n";
    }
}