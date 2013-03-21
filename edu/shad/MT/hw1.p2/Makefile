src = \
		src/main.cpp \
		src/worker.cpp \
		src/crawler.cpp \
		src/link_parser.cpp \
		src/message_processor.cpp \
		src/page_downloader.cpp

src2 = \
		src/simple_main.cpp \
		src/worker.cpp \
		src/crawler.cpp \
		src/link_parser.cpp \
		src/message_processor.cpp \
		src/page_downloader.cpp

obj_list := $(notdir $(src:.cpp=.o))
obj_list2 := $(notdir $(src2:.cpp=.o))

obj_dir := obj

CC := ccache g++-4.6 -std=c++0x
CCFLAGS := -lpthread -Wall -g -pthread
CCFLAGS += -I./src -lcurl -lboost_graph -lboost_regex -lhtmlcxx -lboost_date_time

outfile = crawler
outfile2 = simple_crawler

.PHONY: all
all: $(obj_list) $(obj_list2)
	$(info Linking.)
	@$(CC) $(addprefix $(obj_dir)/, $(obj_list)) $(CCFLAGS) -o $(outfile)
	@$(CC) $(addprefix $(obj_dir)/, $(obj_list2)) $(CCFLAGS) -o $(outfile2)
	$(info All targets complete.)



%.o: src/%.cpp
	$(info Compiling $^)
	@mkdir -p $(obj_dir)
	@$(CC) -c $^ $(CCFLAGS) -o $(obj_dir)/$@

.PHONY: clean
clean:
	@[ ! -e $(obj_dir) ] || rm -rf $(obj_dir)
	@[ ! -e $(outfile) ] || rm $(outfile)
	$(info Cleaning finished.)
