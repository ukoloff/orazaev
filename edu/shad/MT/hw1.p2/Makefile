src = \
		src/worker.cpp \
		src/test_queue.cpp \
		src/link_parser.cpp \
		src/message_processor.cpp \
		src/page_downloader.cpp

obj_list := $(notdir $(src:.cpp=.o))

obj_dir := obj

src2 = \
		src/test_get_domain.cpp \
		src/link_parser.cpp

CC := ccache g++-4.6 -std=c++0x
CCFLAGS := -lpthread -Wall -g -pthread
CCFLAGS += -I./src -lcurl -lboost_graph -lboost_regex -lhtmlcxx

outfile = a.out
outfile2 = test_link_parser.out

.PHONY: all
all: $(obj_list)
	$(info Linking.)
	@$(CC) $(addprefix $(obj_dir)/, $(obj_list)) $(CCFLAGS) -o $(outfile)
	@#$(CC) $(src2) $(CCFLAGS) -o $(outfile2)
	$(info All targets complete.)



%.o: src/%.cpp
	$(info Compiling $^)
	@mkdir -p $(obj_dir)
	@$(CC) -c $^ $(CCFLAGS) -o $(obj_dir)/$@

.PHONY: clean
clean:
	@[ ! -e $(obj_dir) ] || rm -rf $(obj_dir)
	@[ ! -e $(outfile) ] || rm $(outfile)
	@[ ! -e $(outfile2) ] || rm $(outfile2)
	$(info Cleaning finished.)
