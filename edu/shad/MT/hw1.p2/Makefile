src = \
		src/worker.cpp \
		src/test_queue.cpp

CC = g++ -std=c++0x -lpthread -Wall
CCFLAGS = -I./src

outfile = a.out

all: sources
	$(CC) $(src) $(CCFLAGS) -o $(outfile)

sources: $(src)

clean:
	rm *.out
