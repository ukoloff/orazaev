# makefile for babenko dz1_z1.cpp

GXX := g++ -g
out := bin/a.out
objdir := obj

src := dz1_z1.cpp
# obj := $(patsubst %.cpp,%.o,$(source))
obj := $(foreach i,$(src:.cpp=.o),$(objdir)/$i)

.PHONY: all
all: $(out)
	$(info ==> Build is complete!)



$(out): $(obj)
	$(info ==> Compile $(out))
	@$(GXX) $^ -o $(out)



$(obj): $(src)
	$(info ==> Compile $(src))
	$(shell [ ! -d ./$(objdir) ] && mkdir ./$(objdir))
	@$(GXX) $(@F:.o=.cpp) -c -o $@



$(src):
	


.PHONY: clean
clean::
	@rm -rf $(objdir)
	@rm -f $(out)
	$(info ==> Clean complete!)

