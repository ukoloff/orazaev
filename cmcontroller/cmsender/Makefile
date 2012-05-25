program_name	:= cmsender

orazaev_root	:= ../../

CPP			 := g++
libraries	   := 
compile_flags   :=

headers		 := $(orazaev_root)/cmcontroller/cmdefinitions.h \
				$(orazaev_root)/util/socket/TSocket.h
				   
sources		 := main.cpp \
				$(orazaev_root)/util/socket/TSocket.cpp 


all: $(program_name)

$(program_name): 
		$(CPP) -I $(orazaev_root) $(headers) $(compile_flags) || echo "There are no headers in project"; 
		$(CPP) -I $(orazaev_root) $(sources) $(compile_frags) $(libraries) -o $(program_name);

.PHONY: clean

clean:
		rm $(program_name);
