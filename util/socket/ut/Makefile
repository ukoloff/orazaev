orazaev_root := ../../..

GXX := g++ -I $(orazaev_root)
GOBJ := g++ -c -I $(orazaev_root)
FLAGS := -g

mainSrc := $(shell ls *.cpp)
sockSrc :=
sockHeader := ../socket.h

mainObj := $(mainSrc:.cpp=.o)
sockObj := $(sockSrc:.cpp=.o)

out := $(mainSrc:.cpp=.out)



.PHONY: all
all: $(out)
	$(info ==> Cleaning obj...)
	@rm -f *.o
	$(info ==> Complete.)

$(out): obj
	$(GXX) $(FLAGS) $(@:.out=.o) $(notdir $(sockObj)) -o $@

.PHONY: obj
obj: $(sockObj) $(mainObj)

$(sockObj): $(sockSrc)
	$(GOBJ) $(@:.o=.cpp)
	
$(mainObj): $(mainSrc)
	$(GOBJ) $(@:.o=.cpp)

.PHONY: clean
clean::
	@rm -f $(out)
	$(info ==> Cleaning complete.)
