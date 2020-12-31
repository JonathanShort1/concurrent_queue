# concurrent_queue
Implementation and analysis of locking and lock free concurrent queues using C++. The lock-free implementation follows the algorithm descrided in this [paper](https://www.cs.rochester.edu/~scott/papers/1996_PODC_queues.pdf). 

# How to compile:
 - `cd concurrent_queue`
 - `make all`

 # How to run unit tests:
 - `./main -1`

# How to run performance tests:
 - On default number of cores but decide number of threads: `./main <numThreads>`, where numThreads = [1,12]

 - Decide number of cores: `taskset --cpu-list 0 - <numCores + 1> ./main numCores`, where numCores = [1,12].
 - The reason for numCores + 1 is that the main thread uses a core
 - see `man taskset` on linux
