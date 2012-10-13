orazaev_root := ../../..
GXX := g++ -I $(orazaev_root)
GOBJ := g++ -I $(orazaev_root) -c
FLAGS := -g -lpthread

mainSrc := $(shell ls *.cpp)
unitSrc := ../mutex.cpp ../thread.cpp
unitHeader := ../mutex.cpp ../thread.cpp

mainObj := $(mainSrc:.cpp=.o)
unitObj := $(unitSrc:.cpp=.o)

out := $(mainSrc:.cpp=.out)



.PHONY: all
all: $(out)
	$(info ==> Cleaning obj and gch...)
	@rm -f *.o *.h.gch
	@rm -f ../mutex.h.gch
	@rm -f ../thread.h.gch
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
