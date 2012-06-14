program_name	:= utest

orazaev_root	:= ../..

CPP			 := g++
libraries	   := 
compile_flags   := -pedantic -Wall

headers		 := $(orazaev_root)/asciiart/asciiart.h
				   
sources		 := main.cpp \
				$(orazaev_root)/asciiart/asciiart.cpp


all: $(program_name)

$(program_name): 
		$(CPP) -I $(orazaev_root) $(headers) $(compile_flags) || echo "There are no headers in project"; \
		$(CPP) -I $(orazaev_root) $(sources) $(compile_frags) $(libraries) -o $(program_name); \

.PHONY: clean

clean:
		rm $(program_name); 
