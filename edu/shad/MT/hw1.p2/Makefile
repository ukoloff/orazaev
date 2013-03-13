src = \
		src/worker.cpp \
		src/messages.cpp \
		src/test_queue.cpp

src2 = \
		src/test_parser.cpp \
		src/link_parser.cpp

CC = g++ -std=c++0x -lpthread -Wall -g -pthread
CCFLAGS = -I./src -lcurl -lboost_graph -lboost_regex

outfile = a.out
outfile2 = test_link_parser.out

all: sources
	$(CC) $(src) $(CCFLAGS) -o $(outfile)
	$(CC) $(src2) $(CCFLAGS) -o $(outfile2)

sources: $(src) $(src2)

clean:
	rm *.out
