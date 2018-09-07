CC = /usr/bin/g++
CFLAGS = --std=c++17 -Wall -Wextra -O3
LDFLAGS = -lpthread -lboost_thread -lboost_filesystem -lboost_program_options -lboost_system

all: ssfi test_worker

ssfi: src/main.cpp worker.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) $(DFLAGS) src/main.cpp build/worker.o -o ssfi

worker.o: src/worker.cpp
	$(CC) $(CFLAGS) $(INCLUDE) $(DFLAGS) -c src/worker.cpp -o build/worker.o

test_worker: test/test_worker.cpp worker.o
	$(CC) $(CFLAGS) $(INCLUDE) $(DFLAGS) test/test_worker.cpp build/worker.o -o test/test_worker

clean:
	rm -rf ssfi
	rm -rf test/test_worker
	rm -rf build/*
