program_name	:= cmcontroll

orazaev_root	:= ../../

CPP			 := g++
libraries	   := -lpthread
compile_flags   :=

headers		 := \
				cmc_server.h \
				$(orazaev_root)/util/logger/logger.h  \
				$(orazaev_root)/util/socket/TSocket.h \
				$(orazaev_root)/util/thread/TMutex.h \
				$(orazaev_root)/util/thread/TThread.h \
				   
sources		 := \
				main.cpp \
				cmc_server.cpp \
				$(orazaev_root)/util/logger/logger.cpp \
				$(orazaev_root)/util/socket/TSocket.cpp \
				$(orazaev_root)/util/thread/TMutex.cpp \
				$(orazaev_root)/util/thread/TThread.cpp


all: $(program_name)

$(program_name): 
		$(CPP) -I $(orazaev_root) $(headers) $(compile_flags); \
		$(CPP) -I $(orazaev_root) $(sources) $(compile_frags) $(libraries) -o $(program_name); \

.PHONY: clean

clean:
		rm $(program_name); \
        rm *.h.gch;
