src = \
		src/worker.cpp \
		src/messages.cpp \
		src/test_queue.cpp

src2 = \
		test_parser.cpp \
		link_parser.cpp

CC = g++ -std=c++0x -lpthread -Wall
CCFLAGS = -I./src -lcurl -lboost_graph -lboost_regex

outfile = a.out
outfile2 = test_link_parser.out

all: sources
	$(CC) $(src) $(CCFLAGS) -o $(outfile)
	$(CC) $(src2) $(CCFLAGS) -o $(outfile2)

sources: $(src) $(src2)

clean:
	rm *.out
