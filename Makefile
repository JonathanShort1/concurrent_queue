CC=g++
FLAGS= -Wall -Wextra -std=c++11 -pthread

all: main

main: main.cpp locking_queue.cpp locking_queue.h
	$(CC) $(FLAGS) -o main main.cpp locking_queue.cpp



################################################################################
#						CLEAN
################################################################################

clean: 
	rm -rf *.o