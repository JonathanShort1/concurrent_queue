CC=g++
FLAGS= -Wall -Wextra -std=c++11 -pthread -g

all: main

################################################################################
#						MAIN DRIVER
################################################################################

main: main.cpp locking_queue.cpp locking_queue.h lock_free_queue.h \
		lock_free_queue.cpp node.h cqueue.h tests.h tests.cpp \
		performance.h performance.cpp
	$(CC) $(FLAGS) -o main main.cpp locking_queue.cpp lock_free_queue.cpp  \
		tests.cpp performance.cpp



################################################################################
#						CLEAN
################################################################################

clean: 
	rm -rf *.o main