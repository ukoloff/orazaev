program_name	:= cmcontroll

orazaev_root	:= ../../

CPP			 := g++
libraries	   := -lpthread
compile_flags   :=

headers		 := \
				cmc_server.h \
				$(orazaev_root)/cmcontroller/cmdefinitions.h \
                $(orazaev_root)/util/error.h \
				$(orazaev_root)/util/logger/logger.h  \
				$(orazaev_root)/util/socket/socket.h \
				$(orazaev_root)/util/thread/mutex.h \
				$(orazaev_root)/util/thread/thread.h
				   
sources		 := \
				main.cpp \
				cmc_server.cpp \
				$(orazaev_root)/util/logger/logger.cpp \
				$(orazaev_root)/util/thread/mutex.cpp \
				$(orazaev_root)/util/thread/thread.cpp


all: $(program_name)

$(program_name): 
		$(CPP) -I $(orazaev_root) $(sources) $(compile_frags) $(libraries) -o $(program_name);

.PHONY: clean

clean:
		rm $(program_name); 
