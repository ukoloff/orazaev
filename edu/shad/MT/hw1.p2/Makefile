src = \
		src/worker.cpp \
		src/messages.cpp \
		src/test_queue.cpp \
        src/link_parser.cpp \
        src/page_downloader.cpp

src2 = \
		src/test_parser.cpp \
		src/link_parser.cpp

CC = g++-4.6 -std=c++0x -lpthread -Wall -g -pthread
CCFLAGS = -I./src -lcurl -lboost_graph -lboost_regex -lhtmlcxx

outfile = a.out
outfile2 = test_link_parser.out

all: sources
	$(CC) $(src) $(CCFLAGS) -o $(outfile)
	$(CC) $(src2) $(CCFLAGS) -o $(outfile2)

sources: $(src) $(src2)

clean:
	rm *.out
