src = \
		test_page_downloader.cpp \
		page_downloader.cpp

CC = g++ -std=c++0x
CCFLAGS = -I. -lcurl

outfile = test_page_downloader.out

all: sources
	$(CC) $(src) $(CCFLAGS) -o $(outfile)

sources: $(src)

clean:
	rm *.out
