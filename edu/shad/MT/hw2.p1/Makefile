src = \
		src/kmeans.cpp

obj_list := $(notdir $(src:.cpp=.o))

obj_dir := obj

CCACHE := $(shell if which ccache >/dev/null 2>&1; then echo "ccache"; fi)
GXX := $(shell  if which g++-4.7 >/dev/null 2>&1; then echo "g++-4.7 -std=c++0x"; else \
		if which g++-4.6 >/dev/null 2>&1; then echo "g++-4.6 -std=c++0x"; else \
		echo "g++"; fi; fi)

CC := $(CCACHE) $(GXX)
CCFLAGS := -fopenmp -Wall
CCFLAGS += -I./src

outfile = a.out

TESTCANONIC := canonic/kmeans.canonic.txt
TESTDATA := canonic/data.txt
TESTOUT := out.txt
TESTPARAMS := 4 $(TESTDATA) $(TESTOUT)

.PHONY: all
all: $(obj_list)
	$(info Linking.)
	@$(CC) $(addprefix $(obj_dir)/, $(obj_list)) $(CCFLAGS) -o $(outfile)
	$(info All targets complete.)
	$(info ..............................................................)
	$(info Testing...)
	$(shell time ./$(outfile) $(TESTPARAMS))
	$(info $(shell sleep 0.3; \
		if ! diff $(TESTCANONIC) $(TESTOUT) >/dev/null; then \
			echo "[FAILED]"; \
		else \
			echo "[PASSED]"; \
		fi \
	))


%.o: src/%.cpp
	$(info Compiling $^)
	@mkdir -p $(obj_dir)
	@$(CC) -c $^ $(CCFLAGS) -o $(obj_dir)/$@

.PHONY: clean
clean:
	@[ ! -e $(obj_dir) ] || rm -rf $(obj_dir)
	@[ ! -e $(outfile) ] || rm $(outfile)
	@[ ! -e $(TESTOUT) ] || rm $(TESTOUT)
	$(info Cleaning finished.)
