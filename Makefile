CC=g++
FLAGS= -Wall -Wextra -std=c++11 -pthread

all: main

main: main.cpp locking_queue.cpp locking_queue.h lock_free_queue.h lock_free_queue.cpp node.h
	$(CC) $(FLAGS) -o main main.cpp locking_queue.cpp lock_free_queue.cpp



################################################################################
#						CLEAN
################################################################################

clean: 
	rm -rf *.o