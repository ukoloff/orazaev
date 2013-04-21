ROOT_DIR := $(shell echo $$HOME)/git/orazaev

src = \
		src/hw3.p4.cpp \
		MiniCppUnit.cxx

obj_list := $(notdir $(src:.cpp=.o))
obj_list := $(notdir $(obj_list:.cxx=.o))
$(info $(obj_list))


obj_dir := obj

CC := ccache g++-4.6 --std=c++0x -lboost_date_time -g
CCFLAGS := -Wall
CCFLAGS += -I$(ROOT_DIR)
CCFLAGS += -DUNIT_TESTING

outfile = a.out

.PHONY: all
all: $(obj_list)
	$(info Linking.)
	@$(CC) $(addprefix $(obj_dir)/, $(obj_list)) $(CCFLAGS) -o $(outfile)
	$(info All targets complete.)
	$(shell find . | grep -e '\.cpp$$' -e '\.h$$' | xargs ctags --extra=+q)
	$(info ctags regenerated)
	$(info Testing:)
	./$(outfile) < /dev/null



%.o: src/%.cpp
	$(info Compiling $^)
	@mkdir -p $(obj_dir)
	@$(CC) -c $^ $(CCFLAGS) -o $(obj_dir)/$@

MiniCppUnit.o: $(ROOT_DIR)/cppunit/MiniCppUnit.cxx
	$(info Compiling $^)
	@mkdir -p $(obj_dir)
	@$(CC) -c $^ $(CCFLAGS) -o $(obj_dir)/$@

.PHONY: clean
clean:
	@[ ! -e $(obj_dir) ] || rm -rf $(obj_dir)
	@[ ! -e $(outfile) ] || rm $(outfile)
	$(info Cleaning finished.)
