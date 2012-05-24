program_name	:= cmundumper

orazaev_root	:= ../../

CPP			 := g++
libraries	   := 
compile_flags   :=

headers		 := 
				   
sources		 := cmundumper.cpp


all: $(program_name)

$(program_name): 
		$(CPP) -I $(orazaev_root) $(headers) $(compile_flags) || echo "There are no headers in project"; 
		$(CPP) -I $(orazaev_root) $(sources) $(compile_frags) $(libraries) -o $(program_name); \

.PHONY: clean

clean:
		rm $(program_name); \
        rm *.h.gch || echo "There are no *.h.gch in project";
