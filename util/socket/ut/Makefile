GXX := g++
GOBJ := g++ -c
FLAGS := -g

mainSrc := $(shell ls *.cpp)
sockSrc := ../socket.cpp
sockHeader := ../socket.h

mainObj := $(mainSrc:.cpp=.o)
sockObj := $(sockSrc:.cpp=.o)

out := $(mainSrc:.cpp=.out)



.PHONY: all
all: $(out)
	$(info ==> Cleaning obj and gch...)
	@rm -f *.o *.h.gch
	@rm -f ../TSocket.h.gch
	$(info ==> Complete.)

$(out): obj
	$(GXX) $(FLAGS) $(@:.out=.o) $(notdir $(sockObj)) -o $@

.PHONY: obj
obj: $(sockObj) $(mainObj)

$(sockObj): $(sockHeader) $(sockSrc)
	$(GOBJ) $(@:.o=.cpp)
	
$(mainObj): $(sockHeader) $(mainSrc)
	$(GOBJ) $(@:.o=.cpp)

$(sockHeader):
	$(GXX) $(sockHeader)


.PHONY: clean
clean::
	@rm -f $(out)
	$(info ==> Cleaning complete.)
