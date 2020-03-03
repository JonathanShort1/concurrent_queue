
#include <iostream>
#include <cstdlib>

#include "locking_queue.h"
#include "lock_free_queue.h"
#include "performance.h"
#include "tests.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc == 1) {
        cout << "Usage: opt == -1 for testing, opt == (1-12) for performance\n";
        cout << "taskset <number of cores in bitmask> ./main; ./main <number of cores in dec>\n";
        return -1;
    }

    int opt = std::strtol(argv[1], nullptr, 0);
    cout << opt << endl;

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