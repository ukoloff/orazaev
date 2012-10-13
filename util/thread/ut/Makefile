GXX := g++
GOBJ := g++ -c
FLAGS := -g -lpthread

mainSrc := $(shell ls *.cpp)
unitSrc := ../TMutex.cpp ../TThread.cpp
unitHeader := ../TMutex.cpp ../TThread.cpp

mainObj := $(mainSrc:.cpp=.o)
unitObj := $(unitSrc:.cpp=.o)

out := $(mainSrc:.cpp=.out)



.PHONY: all
all: $(out)
	$(info ==> Cleaning obj and gch...)
	@rm -f *.o *.h.gch
	@rm -f ../TMutex.h.gch
	@rm -f ../TThread.h.gch
	$(info ==> Complete.)

$(out): obj
	$(GXX) $(FLAGS) $(@:.out=.o) $(notdir $(unitObj)) -o $@

.PHONY: obj
obj: $(unitObj) $(mainObj)

$(unitObj): $(unitHeader) $(unitSrc)
	$(GOBJ) $(@:.o=.cpp)
	
$(mainObj): $(unitHeader) $(mainSrc)
	$(GOBJ) $(@:.o=.cpp)

$(unitHeader):
	$(GXX) $(unitHeader)


.PHONY: clean
clean::
	@rm -f $(out)
	$(info ==> Cleaning complete.)
